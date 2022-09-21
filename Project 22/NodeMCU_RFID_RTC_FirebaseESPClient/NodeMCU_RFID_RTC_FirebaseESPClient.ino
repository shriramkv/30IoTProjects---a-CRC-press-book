#include <ESP8266WiFi.h> //Library for WiFi functionalities
#include <SPI.h> //Library for I2C functionalities
#include <MFRC522.h> //Library for RC522 RFID
#include <RTClib.h> //Library for RTC module
#include <Firebase_ESP_Client.h> //Library for Firebase
//Helper header files
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

//Your WIFI details
#define WIFI_SSID "YOUR WIFI SSID"
#define WIFI_PASSWORD "YOUR WIFI PASSWORD"

#define RFID_SS D3 //NodeMCU pin to which SDA/SS pin of RFID is connected
#define RFID_RST D4 //NodeMCU pin to which RST pin of RFID is connected
#define redLED D8 //NodeMCU pin to which Red LED is connected
#define greenLED D9 //NodeMCU pin to which Green LED is connected [RX]
#define buzzer D10 //NodeMCU pin to which Buzzer is connected [TX]

MFRC522 rfid(RFID_SS, RFID_RST); //MFRC522 constructor

RTC_DS3231 RTC; //RTC_DS3231 object
DateTime nowDT; //DateTime object
String nowDTStr=""; //Storing the Date-Time in String format

//Firebase Details
#define API_KEY "Web API Key of your Firebase project"
#define DATABASE_URL "DATABASE URL of your Realtime Database"
#define USER_EMAIL "Authenticated User Email ID"
#define USER_PASSWORD "Authenticated User password"

FirebaseData fbdo; //FirebaseData object
FirebaseAuth auth; //FirebaseAuth object
FirebaseConfig config; //FirebaseConfig object

String newNUID = ""; //Stores the scanned NUID
//Array of verified NUIDs
String verifiedNUID[] = {"f9e3f197", "4697d83", "764e281", "a26d97f6", "e39b84c1"};
//Variables used for updating information in Firebase
int attendance;
String IN_OUT;
int totalInside;
bool error = false;

void setup() { 
  Serial.begin(115200); //Initialize NodeMCU's Serial
  pinMode(redLED, OUTPUT); //Direction of Red LED
  pinMode(greenLED, OUTPUT); //Direction of Green LED
  pinMode(buzzer, OUTPUT); //Direction of Buzzer

  //Connecting NodeMCU to WiFi
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi connected.\nNodeMCU's Local IP Address: ");
  Serial.println(WiFi.localIP()); //Print the local IP address of NodeMCU

  SPI.begin(); //Initialize SPI communication
  rfid.PCD_Init(); //Initialize RC522 RFID module
  RTC.begin(); //Initialize DS3231 RTC module
  //Adjusts the time of RTC module when using for the first time or when its power is disconnected.
  if(RTC.lostPower()) {
    //DATE and TIME gets replaced with the date and time of the PC during compilation.
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  config.api_key = API_KEY; //Assign the web API key
  config.database_url = DATABASE_URL; //Assign the Realtime Database URL
  //Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback;

  //Assign the Authenticated user credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth); //Initialize Firebase
  Firebase.reconnectWiFi(true); //Reconnects WiFi if the connection is lost
  
  totalInside = Firebase.RTDB.getInt(&fbdo, "/Total_Inside") ? fbdo.intData() : 0;
}
 
void loop() {

  //Checks whether a PICC is detected and its data is read
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  //Prints the PICC type
  Serial.print("PICC type: ");
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  //Check if the PICC is of Classic MIFARE type
  if(piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println("Your tag is not of MIFARE Classic");
    //Switch ON Red LED & Buzzer for 1 sec
    digitalWrite(redLED, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzer, LOW);
    return;
  }

  //Obtain the NUID of newly scanned PICC
  for (byte i = 0; i < rfid.uid.size; i++) {
    newNUID += String(rfid.uid.uidByte[i], HEX); //NUID in HEX (base-16)
    // newNUID += String(rfid.uid.uidByte[i], DEC); //NUID in DEC (base-10)
  }
  
  if(isVerified(newNUID)) {
    //Obtains the string stored in IN_OUT tag
    if(Firebase.RTDB.getString(&fbdo, ("/"+newNUID+"/IN_OUT").c_str())){
      IN_OUT = fbdo.stringData();
    } else {
      IN_OUT = "OUT";
      Firebase.RTDB.setInt(&fbdo, ("/"+newNUID+"/Attendance").c_str(), 0) ? Serial.println("New Person. Attendance set to 0") : setError(fbdo.errorReason());
    }

    if(IN_OUT == "OUT"){  
      //If a person enters, their personal attendance, time of entry & total number of people inside are updated in Firebase
      Serial.print("A person with ID ");
      Serial.print(newNUID);
      Serial.println(" has entered.");
      attendance = Firebase.RTDB.getInt(&fbdo, ("/"+newNUID+"/Attendance").c_str()) ? fbdo.intData() : setError(fbdo.errorReason());
      Firebase.RTDB.setInt(&fbdo, ("/"+newNUID+"/Attendance").c_str(), ++attendance) ? Serial.println("Attendance Updated") : setError(fbdo.errorReason());
      Firebase.RTDB.setString(&fbdo, ("/"+newNUID+"/IN_OUT").c_str(), "IN") ? Serial.println("IN or OUT Updated") : setError(fbdo.errorReason());
      Firebase.RTDB.pushString(&fbdo, ("/"+newNUID+"/IN_TIME").c_str(), getTimeRTC()) ? Serial.println("Entered Entry time") : setError(fbdo.errorReason()); 
      Firebase.RTDB.setInt(&fbdo, "/Total_Inside", ++totalInside) ? Serial.println("Total Attendance Updated") : setError(fbdo.errorReason());      
    } else if(IN_OUT == "IN") {  
      //If a person exits, time of exit & total number of people inside are updated in Firebase  
      Serial.print("A person with ID ");
      Serial.print(newNUID);
      Serial.println(" has left.");
      Firebase.RTDB.setString(&fbdo, ("/"+newNUID+"/IN_OUT").c_str(), "OUT") ? Serial.println("IN or OUT Updated") : setError(fbdo.errorReason());
      Firebase.RTDB.pushString(&fbdo, ("/"+newNUID+"/OUT_TIME").c_str(), getTimeRTC()) ? Serial.println("Entered Exit time") : setError(fbdo.errorReason());
      Firebase.RTDB.setInt(&fbdo, "/Total_Inside", --totalInside) ? Serial.println("Total Attendance Updated") : setError(fbdo.errorReason());            
    } else 
        error = true;
    
  } else 
      error = true;

  //If there is any error while reading the card or updating data in Firebase then Red LED & buzzer is ON
  //or else Green LED & buzzer is ON 
  if(error){
    digitalWrite(redLED, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzer, LOW);
    error = false;
  } else {
    digitalWrite(greenLED, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(greenLED, LOW);
    digitalWrite(buzzer, LOW);
  }

  newNUID = ""; //Empty the content in newNUID
  Serial.println();
}

//Verifies whether the new NUID is one among the verifiedNUID list
bool isVerified(String newNUID){
  for(int i=0; i < (sizeof(verifiedNUID) / sizeof(verifiedNUID[0])); i++){
    if(newNUID == verifiedNUID[i])
      return true;
  }
  return false;
}

//Obtains the present Date-Time from DS3231 RTC module
String getTimeRTC(){
  nowDT = RTC.now();

  char DATE_TIME_FORMAT[] = "DD/MM/YYYY hh:mm:ss";
  nowDTStr = nowDT.toString(DATE_TIME_FORMAT);
  
  Serial.print("Time: ");
  Serial.println(nowDTStr);
  return nowDTStr;  
}

//Sets the Error flag to true
int setError(String errorReason){
  Serial.print("Failure Occured. Reason: ");
  Serial.println(errorReason); 
  error = true;
  
  return 0;
}

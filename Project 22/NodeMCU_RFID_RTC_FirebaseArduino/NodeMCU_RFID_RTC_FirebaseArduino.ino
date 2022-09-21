#include <ESP8266WiFi.h> //Library for WiFi functionalities
#include <SPI.h> //Library for SPI functionalities
#include <MFRC522.h> //Library for RC522 RFID
#include <RTClib.h> //Library for RTC module
#include <FirebaseArduino.h> //Library for Firebase

//Downgrade the ArduinoJson library to Version 5.13.5 because FirebaseArduino.h is built on it.
//Change the kFirebaseFingerprint in Arduino > libraries > firebase-arduino > src > FirebaseHttpClient.h with the new fingerprint.
//You can find the new fingerprint from https://www.grc.com/fingerprints.htm website.
//Type your xxxx.firebaseio.com (Database URL) in the Custom Site Fingerprinting section and click Fingerprint site

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

#define DATABASE_URL "DATABASE URL of your Realtime Database" //Copy the Realtime Database URL (without HTTPS and /)
#define DATABASE_SECRET "Your Database Secret" //Copy the secret (Project settings > Service Accounts > Database Secrets > Secret)

String newNUID = ""; //Stores the scanned NUID
//Array of verified NUIDs
String verifiedNUID[] = {"f9e3f197", "4697d83", "764e281", "a26d97f6", "e39b84c1"};
//Variables used for updating information in Firebase
int attendance = 0;
String IN_OUT;
int totalInside = 0;
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
  
  Firebase.begin(DATABASE_URL, DATABASE_SECRET);
  totalInside = Firebase.getInt("Total_Inside");
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
    IN_OUT = Firebase.getString(newNUID+"/IN_OUT");

    if(IN_OUT == "OUT" || IN_OUT == ""){
      //If a person enters, their personal attendance, time of entry & total number of people inside are updated in Firebase
      Serial.print("A person with ID ");
      Serial.print(newNUID);
      Serial.println(" has entered.");
      attendance = Firebase.getInt(newNUID+"/Attendance");
      Firebase.setInt(newNUID+"/Attendance", ++attendance);
      Firebase.setString(newNUID+"/IN_OUT", "IN");
      Firebase.pushString(newNUID+"/IN_TIME", getTimeRTC());
      Firebase.setInt("/Total_Inside", ++totalInside);
      if (Firebase.failed()) {
        Serial.print("Updation Failed. Reason: ");
        Serial.println(Firebase.error());  
        error = true;
      } 
    } else if(IN_OUT == "IN") {
        //If a person exits, time of exit & total number of people inside are updated in Firebase  
        Serial.print("A person with ID ");
        Serial.print(newNUID);
        Serial.println(" has left.");
        Firebase.setString(newNUID+"/IN_OUT", "OUT");
        Firebase.pushString(newNUID+"/OUT_TIME", getTimeRTC());
        Firebase.setInt("/Total_Inside", --totalInside);
        if (Firebase.failed()) {
          Serial.print("Updation Failed. Reason: ");
          Serial.println(Firebase.error()); 
          error = true;         
        }
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

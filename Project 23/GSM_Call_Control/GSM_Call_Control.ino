#include <SoftwareSerial.h> //Library for replicating Rx, Tx

SoftwareSerial GSM_Serial(7, 8); //Rx Pin, Tx Pin

int LED = 13; //Pin to which LED is connected
String GSM_data = ""; //Stores the data read from GSM module
String SMS_data = ""; //Stores the data to be sent as an SMS
bool LEDstate = false; //Stores the state of LED

void setup() {
  Serial.begin(9600); //Initialization of Arduino's serial
  GSM_Serial.begin(9600); //Initialization of GSM's serial
  pinMode(LED,OUTPUT); //Define the direction of LED pin
  digitalWrite(LED,LOW); //Initially LED is set to OFF state

  GSM_Serial.println("AT+CPIN?"); //Checks the SIM card presence
  delay(1000);
  GSM_Serial.println("AT+COPS?"); //To know the network operator
  delay(1000);
  GSM_Serial.println("AT+CSQ"); //To know the signal strength
  delay(1000);
}

void loop() {
  if (GSM_Serial.available()>0) { //Checks for any incoming data over GSM serial
    GSM_data = GSM_Serial.readString(); //Reads the data from GSM serial
    GSM_data.trim(); //Removes any trailing or leading white space
    Serial.println(GSM_data); //Print the data read from GSM's serial on the Arduino's serial

    //If there is a call then switch ON or OFF the LED
    if(GSM_data == "RING" && LEDstate == false) {
      digitalWrite(LED, HIGH);
      SMS_data = "LED is switched ON";
      Serial.println("LED is switched ON");
    } else if(GSM_data == "RING" && LEDstate == true) {
      digitalWrite(LED, LOW);
      SMS_data = "LED is switched OFF";
      Serial.println("LED is switched OFF");
    }

    //If call ended then toggle LEDstate & send acknowledgement SMS
    if (GSM_data == "NO CARRIER"){
      LEDstate = !LEDstate;
      sendSMS();
    }
  }
}

//Custom function for sending SMS
void sendSMS(){
  GSM_Serial.println("AT+CMGS=\"xxyyyyyyyyyy\""); //Replace x with country code and y with mobile number
  delay(1000);
  GSM_Serial.print(SMS_data); //SMS text to be sent
  delay(1000);
  GSM_Serial.println((char)26); //ASCII code of CTRL+Z. Acts as a Send key.
  delay(1000);
}

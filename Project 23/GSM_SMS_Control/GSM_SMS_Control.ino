#include <SoftwareSerial.h> //Library for replicating Rx, Tx

SoftwareSerial GSM_Serial(7, 8); //Rx Pin, Tx Pin

int LED = 13; //Pin to which LED is connected
String GSM_data = ""; //Stores the data read from GSM module

void setup() {
  Serial.begin(9600); //Initialization of Arduino's serial
  GSM_Serial.begin(9600); //Initialization of GSM's serial
  pinMode(LED,OUTPUT); //Define the direction of LED pin
  digitalWrite(LED,LOW); //Initially LED is set to OFF state

  GSM_Serial.println("AT+CPIN?"); //Checks the SIM card presence
  delay(1000);
  GSM_Serial.println("AT+COPS?"); //To know the network operator
  delay(1000);
  GSM_Serial.println("AT+CMGF=1"); //Sets the message format to text mode
  delay(1000);
  GSM_Serial.println("AT+CNMI=2,2,0,0,0"); //Indicates new SMS
  delay(1000);
}

void loop() {
  if (GSM_Serial.available()>0) { //Checks for any incoming data over GSM serial
    GSM_data = GSM_Serial.readStringUntil('\n'); //Reads the data from GSM Serial line by line
    GSM_data.trim(); //Removes any trailing or leading white space
    Serial.println(GSM_data); //Print the data read from GSM's serial on the Arduino's serial

    //Depending on the message, toggle the state of LED
    if(GSM_data == "ON") {
      digitalWrite(LED,HIGH);
      sendSMS("LED is switched ON");
      Serial.println("LED is switched ON");
    } else if(GSM_data == "OFF") {
      digitalWrite(LED,LOW);
      sendSMS("LED is switched OFF");
      Serial.println("LED is swicthed OFF");
    }
  }
}

//Custom function for sending SMS
void sendSMS(String SMS_data){
  GSM_Serial.println("AT+CMGS=\"xxyyyyyyyyyy\""); //Replace x with country code and y with mobile number
  delay(1000);
  GSM_Serial.print(SMS_data); //SMS text to be sent
  delay(1000);
  GSM_Serial.println((char)26); //ASCII code of CTRL+Z. Acts as a Send key.
  delay(1000);
}

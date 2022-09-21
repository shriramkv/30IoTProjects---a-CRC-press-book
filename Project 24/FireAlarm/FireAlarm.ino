#include <SoftwareSerial.h> //Library for replicating Rx, Tx

SoftwareSerial GSM_Serial(7, 8); //Rx Pin, Tx Pin

#define MQ2 2 //Pin to which MQ2 gas sensor is connected
#define LM35 A0 //Pin to which LM35 is connected
#define TEMP_THRES 60 //Temperature (in °C) above which alert should be triggered

float VRef = 5.0; //Voltage to which +Vs pin of LM35 is connected
float resolution = (VRef*100)/1023.0; //Calculation of resolution

bool notGasLeak = true; //Status of gas leakage
float tempRead; //Stores the temperature reading

void setup() {
  Serial.begin(9600); //Initialization of Arduino's serial
  GSM_Serial.begin(9600); //Initialization of GSM's serial
  pinMode(MQ2,INPUT); //Define the direction of MQ2 DO pin
  pinMode(LM35,INPUT); //Define the direction of LM35 out pin

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
  notGasLeak = digitalRead(MQ2); //Reads the output from DO of MQ2 gas sensor
  tempRead = analogRead(LM35); //Reads the analog output from LM35
  tempRead *= resolution; //Conversion of analog output to temperature

  if(!notGasLeak) { //If LPG/smoke is detected (MQ2 reading is 0)
    Serial.println("Alert!! Smoke/LPG detected.");
    sendAlert();
  }

  Serial.print("Temperature (in °C): ");
  Serial.println(tempRead);
  
  if(tempRead  > TEMP_THRES) { //If temperature rises above TEMP_THRES
    Serial.println("Alert!! Temperature is rising. Your house might be on fire.");  
    sendAlert();
  }

  delay(200);
}

//Custom function for sending an Alert through call and SMS
void sendAlert() {
  GSM_Serial.println("AT+CMGS=\"xxyyyyyyyyyy\""); //Replace x with country code and y with mobile number
  delay(1000);
  GSM_Serial.print("Emergency!! Your house might be under risk of LPG leakage or fire."); //SMS text to be sent
  delay(1000);
  GSM_Serial.println((char)26); //ASCII code of CTRL+Z. Acts as a Send key.
  delay(5000);
  GSM_Serial.println("ATDxxyyyyyyyyyy;"); //Replace x with country code and y with mobile number
  delay(1000);
}

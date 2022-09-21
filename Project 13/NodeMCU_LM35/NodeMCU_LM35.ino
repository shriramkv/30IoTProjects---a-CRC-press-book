#include <ESP8266WiFi.h> //Library for WiFi functionality
#include <WiFiClientSecure.h> //Library for secure connections

#define LM35Pin A0 //NodeMCU Pin to which LM35 is connected
 
const char* ssid = "YOUR WIFI NAME"; //SSID of your WiFi
const char* password = "YOUR WIFI PASSWORD"; //Password of your WiFi

//Google SpreadSheets Details
const char* host = "script.google.com";
const int HTTPSPort = 443;
String deploymentID = "Your Deployment ID";

String requestURL; //Stores the request URL for sending to web app
String clientReply; //Stores the reply from the web app

int VRef = 3.3; //Voltage to which LM35 is connected
float temperatureVal; //Used to store the read temperatures
float resolution = (VRef*100)/1024.0; //Calculation of resolution
String condition; //Stores the temperature condition

int count = 0; //Counting the number of requests sent to web app

WiFiClientSecure espClient; //Create a secure WiFi client

void setup() {
  Serial.begin(115200); //Initialisation of NodeMCU Serial
  pinMode(LM35Pin,INPUT); //Defining the direction of LM35 Temperature sensor
  
  //Connecting NodeMCU to WiFi
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());  //Print the local IP address of NodeMCU

  espClient.setInsecure(); //Used for HTTPS requests
}

void loop() {

    //Connecting to Client
    if(!espClient.connected()){
      Serial.print("Connecting to Client");
      while (!espClient.connect(host, HTTPSPort)){
        Serial.print(".");
      }   
      Serial.println("");
    }

    //Reading Temperature values from LM35 Temperature sensor
    temperatureVal = analogRead(LM35Pin);
    temperatureVal = temperatureVal*resolution;
    Serial.print("Temperature: ");
    Serial.print(temperatureVal);
    Serial.println(" °C");

    //Assigning condition based on Temperature
    if(temperatureVal <= 20)
      condition = "COLD";
    else if(temperatureVal <= 32)
      condition = "NORMAL";
    else
      condition = "HOT";

    //Request URL for sending to web app
    requestURL = "/macros/s/" + deploymentID + "/exec?temperature=" + String(temperatureVal) + "&condition=" + condition;

    //HTTPS request sent to web app
    espClient.print("GET " + requestURL + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "User-Agent: BuildFailureDetectorESP8266\r\n" + 
                 "Connection: Keep-Alive\r\n\r\n");

    Serial.println("Request sent");

    //Reading the reply from web app
      clientReply = espClient.readString();
      if(clientReply.startsWith("HTTP/1.1 302")){
        Serial.println("Data Published Successfully");  
      } else {
        Serial.println("Data Publish Failed");  
      }

    /*
      if(count >= 70)
        ESP.restart(); //Restart the NodeMCU
      
      delay(2000);
      count += 1;
    */
}

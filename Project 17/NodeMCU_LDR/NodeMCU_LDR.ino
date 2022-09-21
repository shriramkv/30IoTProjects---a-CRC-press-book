#include <ESP8266WiFi.h> //Library for WiFi functionalities
#include <Adafruit_MQTT_Client.h> //Library for Adafruit MQTT Client

//Adafruit Details
#define server "io.adafruit.com"
#define port 1883
#define IO_USERNAME  "AIO_USERNAME"
#define IO_KEY       "AIO_KEY"

String ssid = "WIFI_SSID"; //SSID of your WiFi
String password = "WIFI_PASSWORD"; //Password of your WiFi

WiFiClient esp; //Create a WiFi Client

//Creation of Adafruit MQTT Client
Adafruit_MQTT_Client mqtt(&esp, server, port, IO_USERNAME, IO_KEY);
//Variable for publishing data to Adafruit Feed
Adafruit_MQTT_Publish feed = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME"/feeds/intrusion");

int LDRPin = A0; //Initialize the LDR pin
int Buzzer = D5; //Initialize the Buzzer pin
int LDRvalue; //Stores LDR values
bool dataPublished = false; //Tracks whether the data is published to Adafruit feed or not

void setup()
{
  Serial.begin(115200); //Initialisation of NodeMCU Serial
  pinMode(LDRPin, INPUT); //Defining the direction of LDR
  pinMode(Buzzer, OUTPUT); //Defining the direction of Buzzer

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
  Serial.println(WiFi.localIP()); //Print the local IP address of NodeMCU
}

void loop()
{
  //Connecting to MQTT
  if(!mqtt.connected()){
    Serial.print("Connecting to MQTT");
    while (mqtt.connect()){
      Serial.print(".");
    }
    Serial.println("");
  }

  //Reads and prints the LDR values
  LDRvalue = analogRead(LDRPin);
  Serial.print("LDR Value: ");
  Serial.println(LDRvalue); // Prints the value on serial monitor

  //If intrusion is detected
  if(LDRvalue < 500) {
    //Publish the data to Adafruit feed and continuously ring the buzzer
    while(1){
      if(!dataPublished && feed.publish("YES")){
        Serial.println("Successfully uploaded data to Adafruit Feed.");
        dataPublished = true;
      }
      digitalWrite(Buzzer, HIGH); 
      delay(1000); 
      digitalWrite(Buzzer, LOW); 
      delay(1000); 
    }
  }

 delay(1000);
}

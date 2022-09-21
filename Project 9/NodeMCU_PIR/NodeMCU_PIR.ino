#include <ESP8266WiFi.h> //Library for WiFi functionalities
#include <Adafruit_MQTT_Client.h> //Library for Adafruit MQTT Client

//Adafruit Details
#define server "io.adafruit.com"
#define port 1883
#define IO_USERNAME  "AIO_USERNAME"
#define IO_KEY       "AIO_KEY"

String ssid = "YOUR WIFI NAME"; //SSID of your WiFi
String password = "YOUR WIFI PASSWORD"; //Password of your WiFi

WiFiClient esp; //Create a WiFi Client

//Creation of Adafruit MQTT Client
Adafruit_MQTT_Client mqtt(&esp, server, port, IO_USERNAME, IO_KEY);
//Variable for publishing data to Adafruit Feed
Adafruit_MQTT_Publish feed = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME"/feeds/pir");

int PIR = D6; //Initialize the PIR output pin
int LED = D4; //Initialize the LED pin
int PIR_val = 0; //Used for storing PIR output value

void setup()
{
  Serial.begin(115200); //Initialisation of NodeMCU Serial
  pinMode(PIR, INPUT); //Defining the direction of PIR sensor
  pinMode(LED, OUTPUT); //Defining the direction of LED
  
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

  //Connecting to MQTT
  Serial.print("Connecting to MQTT");
  while (mqtt.connect())
  {
    Serial.print(".");
  }
}

void loop()
{
  if(mqtt.connected()) {
    PIR_val = digitalRead(PIR); //Read the PIR output

    if(PIR_val) { //If movement is detected
      Serial.println("Movement Detected.");
      digitalWrite(LED, HIGH);

      //Publish data to the Adafruit feed
      if(feed.publish("Movement Detected")){
        Serial.println("Successfully data is Uploaded to the Adafruit.\n");
        delay(2000);
      } else
        Serial.println("Sorry, data cannot be Uploaded.\n"); 
            
    } else { //If movement is not detected
      Serial.println("No Movement Detected.");
      digitalWrite(LED, LOW);
    }
  }
}

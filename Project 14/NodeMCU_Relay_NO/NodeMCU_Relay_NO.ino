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
//Variable for subscribing data in an Adafruit Feed
Adafruit_MQTT_Subscribe EBFeed = Adafruit_MQTT_Subscribe(&mqtt, IO_USERNAME"/feeds/electric-bulb");

int relayPin = D1; //Initialize the Relay pin
String dataFeed; //Stores data read from the feed 

void setup()
{
  Serial.begin(115200); //Initialisation of NodeMCU Serial
  pinMode(relayPin, OUTPUT); //Defining the direction of relay
  digitalWrite(relayPin, HIGH); //Initially the electric bulb is set to OFF
  
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

  //Subscribing to Electric Bulb Feed
  mqtt.subscribe(&EBFeed);
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

  //Read data from the Feed
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(2000))) {
    if (subscription == &EBFeed) {
      dataFeed = (char *)EBFeed.lastread;

      Serial.print("Received Data: ");
      Serial.print(dataFeed);

      //Control the electric bulb using relay
      if(dataFeed == "ON")
        digitalWrite(relayPin, LOW);
      if(dataFeed == "OFF")
        digitalWrite(relayPin, HIGH);
    }
  }
}

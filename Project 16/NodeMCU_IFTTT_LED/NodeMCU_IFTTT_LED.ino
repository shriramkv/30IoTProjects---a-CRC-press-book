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
Adafruit_MQTT_Subscribe LEDFeed = Adafruit_MQTT_Subscribe(&mqtt, IO_USERNAME"/feeds/led");

int LEDPin = D5; //Initialize the LED pin
String dataFeed; //Stores data read from the feed 

void setup()
{
  Serial.begin(115200); //Initialisation of NodeMCU Serial
  pinMode(LEDPin, OUTPUT); //Defining the direction of LED
  digitalWrite(LEDPin, LOW); //Initially the LED is set to OFF
  
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

  Serial.print("Connecting to MQTT");

  //Subscribing to LED Feed
  mqtt.subscribe(&LEDFeed);
}

void loop()
{
  //Connecting to MQTT
  if(!mqtt.connected()){
    while (mqtt.connect())
      Serial.print(".");
  }

  //Read data from the Feed
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(2000))) {
    if (subscription == &LEDFeed) {
      dataFeed = (char *)LEDFeed.lastread;

      Serial.print("Received Data: ");
      Serial.print(dataFeed);

      //Control the LED
      if(dataFeed == "ON")
        digitalWrite(LEDPin, HIGH);
      if(dataFeed == "OFF")
        digitalWrite(LEDPin, LOW);
    }
  }
}

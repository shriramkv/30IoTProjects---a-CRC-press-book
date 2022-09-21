#include <TinyGPS++.h> //Library for GPS module
#include <SoftwareSerial.h> //Library for replicating Rx, Tx
#include <ESP8266WiFi.h> //Library for WiFi functionalities
#include <Adafruit_MQTT_Client.h> //Library for Adafruit MQTT Client

//Adafruit Details
#define server "io.adafruit.com"
#define port 1883
#define IO_USERNAME  "AIO_USERNAME"
#define IO_KEY       "AIO_KEY"

TinyGPSPlus gps;  //TinyGPSPlus object
SoftwareSerial gpsSerial(D5, D6); //Rx Pin, Tx Pin
String ssid = "YOUR WIFI NAME"; //SSID of your WiFi
String password = "YOUR WIFI PASSWORD"; //Password of your WiFi
String lat_str, lng_str, loc_link; //Storing location details

WiFiClient esp; //Create a WiFi Client

//Creation of Adafruit MQTT Client
Adafruit_MQTT_Client mqtt(&esp, server, port, IO_USERNAME, IO_KEY);
//Variable for publishing data to Adafruit Feed
Adafruit_MQTT_Publish feed = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME"/feeds/location");

void setup() {
  Serial.begin(115200); //Initialisation of NodeMCU Serial
  gpsSerial.begin(9600); //Initialisation of GPS Serial
  
  //Connecting NodeMCU to WiFi
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());  //Print the local IP address of NodeMCU

  //Connecting to MQTT
  Serial.print("Connecting to MQTT");
  while (mqtt.connect()) {
    Serial.print(".");
  }
}

char charLocation[100]; 
void loop() {
  while (gpsSerial.available() > 0) //Data available in GPS Serial
    if (gps.encode(gpsSerial.read())) { //Read and encode the NMEA data
      if (gps.location.isValid() && mqtt.connected()) { //Check whether GPS location is valid & MQTT is connected
        //Latitude, Longtitude of the location is extracted
        lat_str = String(gps.location.lat() , 6);
        lng_str = String(gps.location.lng() , 6);

        //Google Maps Location link using the latitude and longitude
        loc_link = "https://www.google.com/maps/search/?api=1&query="+lat_str+","+lng_str;

        Serial.println(loc_link); //Print the Google Maps link
        loc_link.toCharArray(charLocation, 100); //Convert Google maps link to character array

        //Publish the location link to the Adafruit
        if (feed.publish(charLocation)) {
          Serial.println("Maps Link Published Successfully");
          delay(2000);
        } else
          Serial.println("Maps Link Publishing Failed");
      }
    }
}

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
Adafruit_MQTT_Publish feed = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME"/feeds/distance");

const int trigPin = D5; //NodeMCU pin to which Trig pin of Ultrasonic Sensor is connected
const int echoPin = D6; //NodeMCU pin to which Echo pin of Ultrasonic Sensor is connected
const int buzzer = D7; //NodeMCU pin to which +ve pin of Buzzer is connected

long duration; //Stores the round trip time taken by the ultrasonic waves
int distance; //Stores the distance of the obstacle from the ultrasonic sensor

void setup() {
  Serial.begin(115200); //Initialisation of NodeMCU Serial
  pinMode(trigPin, OUTPUT); //Defining the direction of TRIG pin
  pinMode(echoPin, INPUT); //Defining the direction of ECHO pin
  pinMode(buzzer, OUTPUT); //Defining the direction of buzzer

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

void loop() {
  
  //Clear the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  //Sets the trigPin HIGH for 10 microseconds for generating Ultrasonic burst
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  //Reads the echoPin to obtain the ultrasonic wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  //Calculating the distance from the round trip time
  distance = duration * 0.034 / 2;

  //Invalid distance
  if(distance < 2 || distance > 400)
    return;  

  //About to overflow
  if(distance <= 15)
    digitalWrite(buzzer, HIGH);

  //Print the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm(s)");

  //Publish data to the Adafruit feed
  if(feed.publish(distance)){
    Serial.println("Successfully data is Uploaded to the Adafruit.\n");
    delay(2000);
  } else
    Serial.println("Sorry, data cannot be Uploaded.\n"); 

  digitalWrite(buzzer, LOW);
}

#include <BlynkSimpleEsp8266.h> //Library for Blynk
#include <DHT.h> //Library for DHT sensor

//Defining the Sensor & relay pins
#define soilMoisturePin A0
#define DHTPin D3
#define rainPin D2
#define relayPin D1

//Defining virtual pins for Blynk app
#define virtualSoilMoisturePin V0
#define virtualHumidityPin V1
#define virtualTemperaturePin V2
#define virtualRainPin V3
#define virtualWaterPumpPin V4
#define virtualTextBoxPin V5

BlynkTimer timer; //BlynkTimer object
DHT dht(DHTPin, DHT11); //DHT object

//Details for connecting to Blynk
char authToken[] = "YOUR AUTH TOKEN";
char ssid[] = "YOUR WIFI NAME";
char password[] = "YOUR WIFI PASSWORD";

//Variables for storing sensor data & controlling water pump
float soilMoisture;
int relHum;
float tempC;
float heatIndxC;
bool rain;
int waterPump;

void setup() {
  Serial.begin(115200); //Initialisation of NodeMCU Serial

  //Defining the direction of sensors & relay GPIO pins
  pinMode(soilMoisturePin, INPUT);
  pinMode(DHTPin, INPUT);
  pinMode(rainPin, INPUT);
  pinMode(relayPin, OUTPUT);

  //Connecting to Blynk
  Blynk.begin(authToken, ssid, password);
  //Used to send data in intervals
  timer.setInterval(2000, callFunc);
  
  dht.begin(); //Initialisation of dht object
}

//Reading data from Blynk
BLYNK_WRITE(virtualWaterPumpPin){   
  waterPump = param.asInt();
  if(waterPump == 1){
     Serial.println("Water Pump Switched ON");
     digitalWrite(relayPin, HIGH);
  } else {
     Serial.println("Water Pump Switched OFF");
     digitalWrite(relayPin, LOW);
  }
}

void loop() {
  Blynk.run(); //For managing Blynk
  timer.run(); //For managing BlynkTimer
}

void callFunc() {
  //Read the soil moisture sensor values and map it between 0 to 100
  soilMoisture = analogRead(soilMoisturePin);
  soilMoisture = map(soilMoisture,1023,0,0,100);

  //Read temperature and humidity values from DHT
  tempC = dht.readTemperature();
  relHum = dht.readHumidity();

  //Validating temeperature and humidity values
  if (isnan(tempC) || isnan(relHum))
    return;

  //Calculate heat index in °C 
  heatIndxC = dht.computeHeatIndex(tempC, relHum, false);

  rain = digitalRead(rainPin); //Read data from rain sensor

  //Print the sensor values
  Serial.print("Mositure : ");
  Serial.print(soilMoisture);
  Serial.print("%  Humidity: ");
  Serial.print(relHum);
  Serial.print("%  Temperature: ");
  Serial.print(tempC);
  Serial.print("°C  Heat Index: ");
  Serial.print(heatIndxC);
  Serial.print("°C  ");

  //Writing on Blynk Virtual Pins
  Blynk.virtualWrite(virtualSoilMoisturePin, soilMoisture);
  Blynk.virtualWrite(virtualHumidityPin, relHum);
  Blynk.virtualWrite(virtualTemperaturePin, tempC);

  if(rain == 0){
    Serial.print("Its Raining  ");
    Blynk.virtualWrite(virtualRainPin, 1);
  } else {
    Serial.print("Not Raining  ");
    Blynk.virtualWrite(virtualRainPin, 2);
  }  

  if((soilMoisture < 50 || relHum < 50) && rain == 1){
    Serial.println("Water the Farm");
    Blynk.virtualWrite(virtualTextBoxPin, "Water the Farm"); 
  } else if((soilMoisture >= 50 && relHum >= 50) || rain == 0) {
    Serial.println("Don't Water the Farm");
    Blynk.virtualWrite(virtualTextBoxPin, "Don't Water the Farm");
  } 
}

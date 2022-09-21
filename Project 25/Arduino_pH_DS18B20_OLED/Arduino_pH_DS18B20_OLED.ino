#include <OneWire.h> //Library for 1-Wire communication protocol
#include <DallasTemperature.h> //Library for Temperature sensor
#include <SoftwareSerial.h> //Library for replicating Rx & Tx
#include <Adafruit_SSD1306.h> //Library for SSD1306 OLED module

//Initialising the required pins
#define tempPin 2
#define BT_Tx 3
#define BT_Rx 5
#define redLED 8
#define yellowLED 9
#define greenLED 10
#define pHpin A0

//Constants used for calculating the pH
#define nSampleAvg 10
#define refVoltage 5.0
#define calRefpH 7.0

#define SCREEN_WIDTH 128 //OLED screen width (in pixels)
#define SCREEN_HEIGHT 64 //OLED screen height (in pixels)
#define OLED_ADDRESS 0x3C //OLED I2C Address

//Used for displaying content on the OLED screen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

SoftwareSerial BTserial(BT_Tx,BT_Rx); //Rx Pin, Tx Pin
//Used for obtaining data from DS18B20 Temperature sensor
OneWire oneWire(tempPin);
DallasTemperature tempSensor(&oneWire);

//Variables used for calculating pH
float cummAValue = 0;
float calVoltage = 2.5;
float calpHvalue;
float pHvoltage;
float pHvalue;
float calibration;
unsigned long start = millis();
float tempValueC = 0; //Stores the temperature in °C

float LR = 0.0; //Stores the lactometer reading
float CLR = 0.0; //Stores the Corrected lactometer reading
String quality = "NA"; //Stores the final milk quality grade

void setup() {
  //Serial.begin(9600);
  BTserial.begin(9600); //Initialization of Bluetooth's serial
  tempSensor.begin(); //Initialization of DS18B20 Temperature sensor
  //Defining the direction of GPIO pins
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(pHpin, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS); //Initialize OLED module

  //Initial display configuration
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();

  //pH probe calibration
  display.setCursor(2,20);
  display.println("Place the pH probe in water and wait for 1 minute.");
  display.display();
  delay(3000);
  
  while(millis() - start <= 60000){
    cummAValue = 0;
    for (int i = 0; i < nSampleAvg; i++)
    {
      cummAValue += analogRead(pHpin);
      delay(300);
    }
    calVoltage = (cummAValue/nSampleAvg) * (refVoltage/1023.0);
  }
  calpHvalue = calVoltage * 2.8;
  calibration = calRefpH - calpHvalue;

  //Indicating the completion of pH probe calibration using LEDs
  digitalWrite(redLED, HIGH);
  digitalWrite(yellowLED, HIGH);
  digitalWrite(greenLED, HIGH);
  delay(3000);

  display.clearDisplay();
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  delay(1000);
}

void loop() {  
  //Obtaining the pH value
  cummAValue = 0;
  for (int i = 0; i < nSampleAvg; i++)
  {
    cummAValue += analogRead(pHpin);
    delay(300);
  }
  pHvoltage = (cummAValue/nSampleAvg) * (refVoltage/1023.0);
  pHvalue = (pHvoltage * 2.8) + calibration;
  //pHvalue = calRefpH + ((pHvoltage - calVoltage)*2.8);
  /*Serial.print("pH= ");
  Serial.print(pHvalue);*/

  //Reading the temperature in °C
  tempSensor.requestTemperatures();
  tempValueC = tempSensor.getTempCByIndex(0);

  /*Serial.print(" Temperature: ");
  Serial.print(tempValueC);
  Serial.print(" °C");*/

  //Receiving LR from Bluetooth and calculating the CLR
  if(BTserial.available() > 0){
    LR = BTserial.readString().toFloat();
    if(tempValueC > 0)
      CLR = LR + (tempValueC - 20)*0.2;
    /*Serial.print(" LR: ");
    Serial.print(LR); 
    Serial.print(" CLR: ");
    Serial.print(CLR);*/ 
  }

  //Assessing the final quality of milk using pH and CLR
  if(CLR)
    if((pHvalue <= 6.0 or pHvalue >= 7.3) or (CLR <= 21 or CLR >= 37)){
      quality = "VERY BAD";
      digitalWrite(redLED, HIGH);
      delay(2000);
      digitalWrite(redLED, LOW);
    } else if((pHvalue <= 6.3 or pHvalue >= 7.0) or (CLR <= 24 or CLR >= 34)){
      quality = "BAD";
      digitalWrite(yellowLED, HIGH);
      delay(2000);
      digitalWrite(yellowLED, LOW);
    } else if((pHvalue <= 6.5 or pHvalue >= 6.8) or (CLR <= 26 or CLR >= 32)){
      quality = "AVERAGE";
      digitalWrite(greenLED, HIGH);
      delay(2000);
      digitalWrite(greenLED, LOW);
    } else if((pHvalue > 6.5 and pHvalue < 6.8) and (CLR > 26 and CLR < 32)){
      quality = "GOOD";
      digitalWrite(greenLED, HIGH);
      delay(2000);
      digitalWrite(greenLED, LOW);
    }
  /*Serial.print(" Quality: ");
  Serial.println(quality);*/

  //Printing all the required details on the OLED screen
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("pH: ");
  display.setCursor(20,0);
  display.print(pHvalue);
  display.setCursor(0,15);
  display.print("Temperature: ");
  display.setCursor(80,15);
  display.print(tempValueC);
  display.setCursor(0,30);
  display.print("LR: ");
  display.setCursor(30,30);
  display.print(LR);
  display.setCursor(60,30);
  display.print("CLR: ");
  display.setCursor(90,30);
  display.print(CLR);
  display.setCursor(30,45);
  display.print(quality);
  display.display();

  delay(3000);
}

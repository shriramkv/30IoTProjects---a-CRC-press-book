#include <Wire.h> //Library for I2C functionalities
#include <RTClib.h> //Library for RTC module
#include <Adafruit_SSD1306.h> //Library for OLED

#define SCREEN_WIDTH 128 //OLED screen width (in pixels)
#define SCREEN_HEIGHT 64 //OLED screen height (in pixels)
#define OLED_ADDRESS 0x3C //OLED I2C Address

#define ALARM_RING_TIME 1 //Alarm ring time (in minutes)

//Used for displaying content on OLED screen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

RTC_DS3231 RTC; //RTC_DS3231 object
DateTime nowDT; //Used for storing Date-Time obtained from RTC module

int numberChange = 2; //CHANGE push button pin
int numberSet = 3; //SET push button pin
int buzzer = 4; //Initialize Buzzer pin

//Stores the values read from the Buttons
bool numberChangeState;
bool numberSetState;

String nowDTStr=""; //Storing the Date-Time in String format
String nxtAlarm = "20/06/2025 00:00"; //Set your next alarm Date-Time
int alarmValues[5]; //Stores the Date-Time values individually

//Used to set Date-Time for new Alarm 
int currNum = 0;
int currPos = 0;
String newAlarm = "0*/**/**** **:**";
char tempChar[2] = "";

long startTime; //Used for storing Alarm start time

void setup() {
  Serial.begin(9600); //Initialization of Arduino's Serial
  pinMode(numberChange, INPUT); //Direction of Number Change Button
  pinMode(numberSet, INPUT); //Direction of Number Set Button
  pinMode(buzzer, OUTPUT); //Direction of Buzzer

  RTC.begin(); //Initialize RTC 
  //Adjusts the time of RTC module when using it for the first time or when its power is disconnected.
  if(RTC.lostPower()) {
    //DATE and TIME gets replaced with the date and time of the PC during compilation.
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } 
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS); //Initialize OLED module

  //Initial display configuration
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();  

  //Obtaining the individual Date-Time values from Alarm string
  alarmValues[0] = nxtAlarm.substring(0,2).toInt();
  alarmValues[1] = nxtAlarm.substring(3,5).toInt();
  alarmValues[2] = nxtAlarm.substring(6,10).toInt();
  alarmValues[3] = nxtAlarm.substring(11,13).toInt();
  alarmValues[4] = nxtAlarm.substring(14).toInt();
}

void loop() {
  //Used to obtain the current Date-Time
  nowDT = RTC.now();

  //Format of Date-Time to be displayed
  char DATE_TIME_FORMAT[] = "DD/MM/YYYY hh:mm:ss";
  Serial.print("Present Date-Time: ");
  Serial.println(nowDT.toString(DATE_TIME_FORMAT));

  //Displays the present Date-Time on the OLED
  display.setCursor(0,0);
  display.println("Clock: ");
  display.println(nowDT.toString(DATE_TIME_FORMAT));

  //Displays the next Alarm on the OLED
  display.setCursor(0,20);
  display.println("Next Alarm: ");
  display.println(nxtAlarm);

  //Reads the values from button pins
  numberChangeState = digitalRead(numberChange);
  numberSetState = digitalRead(numberSet);

  //Used to set new Alarm Date-Time using Number Change and Number Set Buttons
  display.setCursor(0,40);
  display.println("New Alarm: ");

  if(numberChangeState == HIGH){
    currNum += 1;
    if(currNum > 9)
      currNum = 0;
    String(currNum).toCharArray(tempChar, 2);
    newAlarm[currPos] = tempChar[0];
  }

  if(numberSetState == HIGH && currPos <= 15) {
    String(currNum).toCharArray(tempChar, 2);
    newAlarm[currPos] = tempChar[0];
    currPos += 1;

    if(currPos == 2 || currPos == 5 || currPos == 10 || currPos == 13){
      currPos += 1;
    }
    
    currNum = 0;
    newAlarm[currPos] = '0';
  }

  display.println(newAlarm);
  display.display();
  delay(500);  
  display.clearDisplay();

  if(currPos > 15){
    alarmValues[0] = newAlarm.substring(0,2).toInt();
    alarmValues[1] = newAlarm.substring(3,5).toInt();
    alarmValues[2] = newAlarm.substring(6,10).toInt();
    alarmValues[3] = newAlarm.substring(11,13).toInt();
    alarmValues[4] = newAlarm.substring(14).toInt();

    if(!DateTime(alarmValues[2], alarmValues[1], alarmValues[0], alarmValues[3], alarmValues[4]).isValid())
      nxtAlarm = "INVALID";  
    else if(alarmValues[2] < nowDT.year())
      nxtAlarm = "INVALID";
    else if(alarmValues[2] == nowDT.year() && alarmValues[1] < nowDT.month())
      nxtAlarm = "INVALID";
    else if(alarmValues[2] == nowDT.year() && alarmValues[1] == nowDT.month() && alarmValues[0] < nowDT.day())
      nxtAlarm = "INVALID";
    else if (alarmValues[2] == nowDT.year() && alarmValues[1] == nowDT.month() && alarmValues[0] == nowDT.day() && alarmValues[3] < nowDT.hour())
      nxtAlarm = "INVALID";
    else if (alarmValues[2] == nowDT.year() && alarmValues[1] == nowDT.month() && alarmValues[0] == nowDT.day() && alarmValues[3] == nowDT.hour() && alarmValues[4] <= nowDT.minute())
      nxtAlarm = "INVALID";
    else {
      nxtAlarm = newAlarm;
      display.setCursor(10,20);
      display.println("Alarm successfully changed.");
      display.display();
      delay(2000);
      display.clearDisplay();
    }

    currNum = 0;
    currPos = 0;
    newAlarm = "0*/**/**** **:**";
  }

  //Activate the Buzzer when its Alarm Date-Time
  if(DateTime(alarmValues[2], alarmValues[1], alarmValues[0], alarmValues[3], alarmValues[4]) == nowDT && nowDT.second() < 3){
      display.setCursor(1,20);
      display.print("Alarm is ringing!");
      display.display();
      display.clearDisplay();
            
      startTime = millis();
      while(!numberSetState && millis() - startTime < ALARM_RING_TIME*60000){
        numberSetState = digitalRead(numberSet); 
        digitalWrite(buzzer, HIGH);
        delay(500); 
        digitalWrite(buzzer, LOW);
        delay(500); 
      }  
      nxtAlarm = "INVALID";

      display.setCursor(1,20);
      display.print("Alarm Ended.");
      display.display();
      delay(1000);
      display.clearDisplay();
  }
}

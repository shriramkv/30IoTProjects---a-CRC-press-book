#include <RTClib.h> //Library for RTC modules

RTC_DS3231 RTC; //RTC_DS3231 object

void setup() { 
  Serial.begin(9600); //Initialisation of Arduino's Serial
  RTC.begin(); //Initialisation of RTC module
  //Date and Time of RTC is adjusted with the date and time on your computer.
  RTC.adjust(DateTime(__DATE__, __TIME__));
  
  Serial.println("Date & Time are SET in RTC Module");
}
 
void loop() {
  /*No Code Required*/
}

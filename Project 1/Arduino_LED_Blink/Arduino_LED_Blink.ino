int LEDPin = 9; //Arduino Digital Pin 9

void setup() {
  pinMode(LEDPin,OUTPUT);  //Set the direction of LEDpin
  digitalWrite(LEDPin,LOW);//Initially, keep the LED OFF
}

void loop() {
  digitalWrite(LEDPin,HIGH); //Write HIGH to ON the LED
  delay(1000);               //Wait for a second
  digitalWrite(LEDPin,LOW);  //Write LOW to OFF the LED
  delay(1000);               //Wait for a second
}

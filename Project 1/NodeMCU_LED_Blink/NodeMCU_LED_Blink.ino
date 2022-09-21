int LEDPin = D2; //NodeMCU Digital Pin 2

void setup() {
  pinMode(LEDPin,OUTPUT);  //Determine the direction of IO pin
  digitalWrite(LEDPin,LOW);//Keep the LED OFF
}

void loop() {
  digitalWrite(LEDPin,HIGH);  // Write HIGH to ON the LED
  delay(1000);               //Wait for a second
  digitalWrite(LEDPin,LOW);  // Write LOW to OFF the LED
  delay(1000);               // Wait for a second
}

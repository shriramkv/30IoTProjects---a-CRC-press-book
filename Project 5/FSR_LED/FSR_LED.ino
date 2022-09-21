int FSRpin = A0; // Initialize the FSR pin
int LEDpin = 11; // Initialize the LED pin

int FSRvalue; // Variable to store the FSR value
int LEDbrightness; // Variable to store the brightness of LED

void setup() {
  Serial.begin(9600); //Start the Serial communication
  pinMode(FSRpin,INPUT); //Set the mode of FSR as INPUT
  pinMode(LEDpin,OUTPUT); //Set the mode of LED as OUTPUT
  digitalWrite(LEDpin,LOW); //Set the LED OFF at the start
}

void loop() {
  FSRvalue = analogRead(FSRpin); //Read the FSR values
  Serial.print("FSR Value: "); //Print the FSR values
  Serial.println(FSRvalue);

  // Map the FSR values from 0-1023 to 0-255
  LEDbrightness = map(FSRvalue, 0, 1023, 0, 255);
  analogWrite(LEDpin, LEDbrightness); //Write the brightness value on LED
  
  delay(100);
}

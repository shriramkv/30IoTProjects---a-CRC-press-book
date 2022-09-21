int LDRPin = A0; //Select the input pin for LDR
int LDRValue = 0; //Variable to store the input value
int streetLight = 7; //Pin of light that supplies artificial sunlight

void setup() {
  Serial.begin(9600); //Begins the Serial Communication
  pinMode(LDRPin, INPUT); //Set A0 as Input Port for receiving LDR values
  pinMode(streetLight, OUTPUT); //Set Digital Pin 7 as Output Port
}

void loop() {
  LDRValue = analogRead(LDRPin); // Reads the value from the sensor
  Serial.print("LDR Value: ");
  Serial.println(LDRValue); // Prints the value on serial monitor

  if (LDRValue < 700) { //Check the light intensity
    Serial.println("There is no enough environmental light. So, LED is getting ON");
    digitalWrite(streetLight, HIGH);
  } else {
    Serial.println("There is enough environmental light. So, LED is getting OFF");
    digitalWrite(streetLight, LOW);
  }
  delay(500);
}

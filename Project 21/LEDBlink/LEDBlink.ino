void setup() {
  Serial.begin(9600); //Initialize Arduino's Serial
  //Initialize digital pin LED_BUILTIN as an output
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  //Turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED is switched ON");
  delay(1000); //Wait for a second
  //Turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED is switched OFF");
  delay(1000); //Wait for a second
}

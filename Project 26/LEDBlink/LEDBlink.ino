#define LED 4 

void setup() {
  //Initialize digital pin LED_BUILTIN as an output
  pinMode(LED, OUTPUT);
}

void loop() {
  //Turn the LED on (HIGH is the voltage level)
  digitalWrite(LED, HIGH);
  delay(1000); //Wait for a second
  //Turn the LED off by making the voltage LOW
  digitalWrite(LED, LOW);
  delay(1000); //Wait for a second
}

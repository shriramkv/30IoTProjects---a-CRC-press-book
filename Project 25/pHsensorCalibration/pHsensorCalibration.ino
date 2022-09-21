#define pHpin A0
float pHvoltage;

void setup() {
  Serial.begin(9600);
}

void loop() {
  pHvoltage = analogRead(pHpin) * (5.0 / 1023.0);
  Serial.print(pHvoltage);
  Serial.println(" V");
  delay(500);
}

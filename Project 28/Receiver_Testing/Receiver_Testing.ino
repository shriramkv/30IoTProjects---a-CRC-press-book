String rcvdStr;
int commaIndex, secCommaIndex;
float gyroX, gyroY, gyroZ;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    rcvdStr = Serial.readStringUntil('\n');
    commaIndex = rcvdStr.indexOf(',');
    secCommaIndex = rcvdStr.indexOf(',', commaIndex + 1);

    gyroX = rcvdStr.substring(0, commaIndex).toFloat();
    gyroY = rcvdStr.substring(commaIndex + 1, secCommaIndex).toFloat();
    gyroZ = rcvdStr.substring(secCommaIndex + 1).toFloat();

    Serial.print(gyroX);
    Serial.print(",");
    Serial.print(gyroY);
    Serial.print(",");
    Serial.println(gyroZ);
  }
}

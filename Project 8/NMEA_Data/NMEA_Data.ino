#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(D5, D6); //Rx Pin, Tx Pin of NodeMCU

void setup() {
  Serial.begin(115200); //Initialisation of NodeMCU's Serial
  gpsSerial.begin(9600); //Initialisation of GPS's Serial
}

void loop() {
  while (gpsSerial.available() > 0) //If data is available on GPS's Serial
    Serial.write(gpsSerial.read()); // then write on NodeMCU's Serial
}

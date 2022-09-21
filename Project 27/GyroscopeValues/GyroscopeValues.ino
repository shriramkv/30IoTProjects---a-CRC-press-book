#include <TinyWireM.h>
#include <SoftwareSerial.h>
SoftwareSerial BTserial(1,3);

#define I2CAddr 0x68
#define FSrange 0x08
#define SSF 65.5

float gyroX, gyroY, gyroZ;

void setup() {
  BTserial.begin(9600);
  TinyWireM.begin();
  TinyWireM.beginTransmission(I2CAddr);
  TinyWireM.write(0x6B);
  TinyWireM.write(0x00);
  TinyWireM.endTransmission(true);
  TinyWireM.beginTransmission(I2CAddr);
  TinyWireM.write(0x1B);
  TinyWireM.write(FSrange);
  TinyWireM.endTransmission(true);
}

void loop() {
  TinyWireM.beginTransmission(I2CAddr);
  TinyWireM.write(0x43);
  TinyWireM.endTransmission(false);
  TinyWireM.requestFrom(I2CAddr,6);
  gyroX = (TinyWireM.read()<<8 | TinyWireM.read()) / SSF;
  gyroY = (TinyWireM.read()<<8 | TinyWireM.read()) / SSF;
  gyroZ = (TinyWireM.read()<<8 | TinyWireM.read()) / SSF;

  BTserial.print(gyroX);
  BTserial.print(",");
  BTserial.print(gyroY);
  BTserial.print(",");
  BTserial.println(gyroZ);
  delay(100); 
}

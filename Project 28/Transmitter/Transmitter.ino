#include <TinyWireM.h> //I2C library for ATtiny
#include <SoftwareSerial.h> //Library for replicating Rx & Tx
SoftwareSerial BTserial(1,3); //Rx Pin, Tx Pin

#define I2CAddr 0x68 //I2C address of MPU6050
#define FSrange 0x08 //Full scale range of gyroscope
#define SSF 65.5 //Sensitivity Scale factor

float gyroX, gyroY, gyroZ; //Variables for storing gyroscope values

void setup() {
  BTserial.begin(9600); //Initialisation of Bluetooth serial
  //Initial I2C and MPU6050 setup
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
  //Read the gyro values from MPU6050
  TinyWireM.beginTransmission(I2CAddr);
  TinyWireM.write(0x43);
  TinyWireM.endTransmission(false);
  TinyWireM.requestFrom(I2CAddr,6);
  gyroX = (TinyWireM.read()<<8 | TinyWireM.read()) / SSF;
  gyroY = (TinyWireM.read()<<8 | TinyWireM.read()) / SSF;
  gyroZ = (TinyWireM.read()<<8 | TinyWireM.read()) / SSF;

  //Print the gyroscope values on Bluetooth serial
  BTserial.print(gyroX);
  BTserial.print(",");
  BTserial.print(gyroY);
  BTserial.print(",");
  BTserial.println(gyroZ);
  delay(100); 
}

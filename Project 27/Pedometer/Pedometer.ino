#include <TinyOzOLED.h> //OLED library for ATtiny
#include <TinyWireM.h> //I2C library for ATtiny

#define THRESMIN -180 //Z-direction gyroscope minimum for step count
#define THRESMAX 180 //Z-direction gyroscope maximum for step count
#define I2CAddr 0x68 //I2C address of MPU6050
#define FSrange 0x08 //Full scale range of gyroscope
#define SSF 65.5 //Sensitivity Scale factor

//Variables used for calculating steps
float gyroX, gyroY, gyroZ;
float minZ, maxZ;
bool pos = false, neg = false;
int steps = 0;
char charArrSteps[40];

void setup() {
  //Initial OLED setup
  OzOled.init();
  OzOled.clearDisplay();
  OzOled.setNormalDisplay();
  OzOled.sendCommand(0xA1);
  OzOled.sendCommand(0xC8);
  
  ("Steps: " + String(steps)).toCharArray(charArrSteps, 40);
  OzOled.printString(charArrSteps, 1, 2,sizeof(charArrSteps));

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
  
  //Calculate the Minimum Value & Maximum value of the hand movement
  if(gyroZ > maxZ){
    maxZ = gyroZ;
  }
  if(gyroZ < minZ){
    minZ = gyroZ;
  }

  if(maxZ > THRESMAX){
    pos = true;    
  }
  if(minZ < THRESMIN){
    neg = true;    
  }

  //After detection of a step, update the parameters and print the number of steps on OLED
  if (pos && neg){
    steps++;
    ("Steps: " + String(steps)).toCharArray(charArrSteps, 40);
    OzOled.printString(charArrSteps, 1, 2,sizeof(charArrSteps));
    pos = false;
    neg = false;
    minZ = 0;
    maxZ = 0;
  }
}

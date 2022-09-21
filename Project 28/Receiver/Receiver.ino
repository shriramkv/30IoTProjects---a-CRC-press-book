#include <Servo.h> //Library for servo motor

//Digital pins to which LEDs & servo motor is connected
#define RED 3
#define YELLOW 5
#define GREEN 7
#define SERVO 9

//Helper constant values
#define CRITICALTHRES 300
#define SEVERETHRES 150
#define MODERATETHRES 75
#define ARRAYLEN 15

#define initPos 180
#define Rot1 40
#define Rot2 25
#define Rot3 15

Servo myServo; //Servo class object

//Variables used in the determination of danger area
String rcvdStr;
int commaIndex, secCommaIndex;
float gyroX, gyroY, gyroZ, minValue, maxValue, oscl;
int i = 0, j = 0;
bool isP = true, isN = true;
float xarrP[ARRAYLEN] = {0.0};
float xarrN[ARRAYLEN] = {0.0};

void setup() {
  Serial.begin(9600); //Initialization of Arduino's Serial
  //Defining the direction of operation of LEDs
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  myServo.attach(SERVO); //Linking to the Arduino Uno’s PWM pin
  myServo.write(initPos); //Initial position of servo motor
}

void loop() {
  if (Serial.available() > 0) { 
    //Split the received data from transmitter with comma as separator 
    rcvdStr = Serial.readStringUntil('\n');
    commaIndex = rcvdStr.indexOf(',');
    secCommaIndex = rcvdStr.indexOf(',', commaIndex + 1);

    gyroX = rcvdStr.substring(0, commaIndex).toFloat();
    gyroY = rcvdStr.substring(commaIndex + 1, secCommaIndex).toFloat();
    gyroZ = rcvdStr.substring(secCommaIndex + 1).toFloat();

    //Store the positive and negative X-direction gyroscope values in their respective arrays
    if(gyroX >= 0) {
      xarrP[i] = gyroX;
      i += 1;
    }
    if(gyroX < 0) {
      xarrN[i] = gyroX;
      i += 1;
    }

    //Finding the maximum and minimum gyroscope value in X-direction
    if (gyroX >= 0 and isN) {
      minValue = xarrN[0];
      for (j = 0; j < ARRAYLEN; j++) {
        if (xarrN[j] < minValue) {
          minValue = xarrN[j];
        }
      }
      Serial.print("minValue: ");
      Serial.println(minValue);
      isP = true;
      isN = false;
      i = 0;
    }
    
    if (gyroX < 0 and isP) {
      maxValue = xarrP[0];
      for (j = 0; j < ARRAYLEN; j++) {
        Serial.print(xarrP[j]);
        if (xarrP[j] > maxValue) {
          maxValue = xarrP[j];
        }
        Serial.print(',');
      }
      Serial.println("");
      Serial.print("maxValue: ");
      Serial.println(maxValue);
      isP = false;
      isN = true;
      i = 0;
    }

    oscl = maxValue + abs(minValue); //Calculate the OSCILLATIONSUM value

    //Rotate the servo motor according to the OSCILLATIONSUM
    //Also, ON the respective category LED
    if (oscl > CRITICALTHRES) {
      myServo.write(initPos - Rot1);
      digitalWrite(RED, HIGH);
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, LOW);
    } else if (oscl > SEVERETHRES) {
      myServo.write(initPos - Rot2);
      digitalWrite(RED, LOW);
      digitalWrite(YELLOW, HIGH);
      digitalWrite(GREEN, LOW);
    } else if (oscl > MODERATETHRES) {
      myServo.write(initPos - Rot3);
      digitalWrite(RED, LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, HIGH);
    } else {
      myServo.write(initPos);
      digitalWrite(RED, LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, LOW);
    }

    //Reset the index of the arrays
    if (i >= ARRAYLEN)
      i = 0;
  }
}

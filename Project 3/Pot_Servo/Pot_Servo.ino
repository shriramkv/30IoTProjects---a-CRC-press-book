#include <Servo.h> //Import the servo library

Servo myServo;  //Servo class object
int servo = 6;  //Initialize the Servo motor pin
int servoAngle; 

int pot = A5;   //Initialize the potentiometer pin
int potVal = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pot, INPUT);    //Declare pot as INPUT
  myServo.attach(servo);  //Linking to the Arduino Uno’s PWM pin
  myServo.write(0);       //Initially the servo motor is set at 0°
}

void loop() {
  potVal = analogRead(pot);  //Read the input from the potentiometer pin
  Serial.print("Potentiometer Value: ");
  Serial.println(potVal);
  servoAngle = map(potVal, 200, 1023, 0, 179); //Map the potentiometer to match the servo angle 
  Serial.print("Servo Angle: ");
  Serial.println(servoAngle);
  myServo.write(servoAngle);    //Finally write the mapped value to the servo for turning.         
  delay(500);      //Give a delay of 500 milli seconds to stabilize the turn.
}

#include <Servo.h> // Import the servo library

Servo myServo;     //Servo class object
int servoPin = 5;  //Initialize servo motor pin

int IRpin = 2; //Initialize IR sensor pin
int IRval; //Used for storing the IR sensor value

int LEDpin = 11; //Initialize LED pin

void setup() {
  Serial.begin(9600); //Begin the Serial communication
  myServo.attach(servoPin); //Linking to the Arduino Uno’s PWM pin
  myServo.write(0); //Initially Servo will be at 0°
  pinMode(IRpin, INPUT); //Define the direction of IR sensor
  pinMode(LEDpin, OUTPUT); //Define the direction of LED
}

void loop() {
  IRval = digitalRead(IRpin); //Store the IR sensor output
  Serial.print("IR sensor: ");
  Serial.println(IRval); //Print the IR sensor output

  if(IRval == 0) { //If anything is infront of IR sensor
    Serial.println("Hand detected. Releasing Soap / Sanitizer.");
    digitalWrite(LEDpin, HIGH); //Turn ON LED
    myServo.write(150); //Rotate the servo motor to 150°
    delay(5000); //Keep the servo motor at 150° for 5 seconds
    digitalWrite(LEDpin, LOW); //Turn OFF LED
    myServo.write(0); //Again bring back the servo motor to 0° 
  }
  
  delay(2000); //Check the IR sensor output for every 2 seconds
}

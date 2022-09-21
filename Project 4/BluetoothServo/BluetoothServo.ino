#include <Servo.h> // Import the servo library

Servo myServo;     //Servo class object
int servoPin = 5;  //Initialize the Servo motor pin

int GreenLED = 12; //Initialize the Green LED to pin 12 of Arduino
int RedLED = 9; //Initialize the Red LED to pin 9 of Arduino

String pswd = "MY SECRET PASSWORD"; //String to store your password
String recvPswd = ""; //String to store the received password

bool doorState = false; //Check whether door is open or close. Initially Closed.
int wrngPswdCounter = 0; //Counter for number of wrong attempts

void setup() {
  Serial.begin(9600); //Switch on Serial communication
  pinMode(GreenLED,OUTPUT); //Set the mode of Green LED as OUTPUT
  pinMode(RedLED,OUTPUT); //Set the mode of Red LED as OUTPUT
  digitalWrite(GreenLED,LOW); //Let the Green LED stay off at the start
  digitalWrite(RedLED,LOW); //Let the Red LED stay off at the start
  
  myServo.attach(servoPin); //Linking to the Arduino Uno’s PWM pin 
  myServo.write(0); //Initially Servo will be at 0° (Closed Door)
}

void loop() {
  if(Serial.available() > 0){ //Checks whether any information is available in Serial or not
    recvPswd = Serial.readString(); //Read the string
    recvPswd.trim(); //Remove all the extra spaces at the beginning and at the end of the String. It removes /r /n at the end of recvPswd
    Serial.print("You have entered the Password: ");
    Serial.println(recvPswd);
  }
  
  if(recvPswd == ""){ //If No password is available then the loop() function starts from beginning.
    return;  
  }
  
  if(recvPswd == pswd) { //Compare the passwords
    if(doorState == false){ //If the door is closed then open it.
      Serial.println("You have entered CORRECT password !! Door is Opening.");
      digitalWrite(GreenLED, HIGH);
      delay(1000);
      for(int ang = 0; ang <= 90; ang++){
        myServo.write(ang);
        delay(20);  
      }
      delay(1000);
      digitalWrite(GreenLED, LOW);
      doorState = true;
    } 
    
    else { //If the door is opened then close it.
      Serial.println("You have entered CORRECT password !! Door is Closing.");
      digitalWrite(GreenLED, HIGH);
      delay(1000);
      for(int ang = 90; ang >= 0; ang--){
        myServo.write(ang);
        delay(20);  
      }
      delay(1000);
      digitalWrite(GreenLED, LOW);
      doorState = false;
    }
    recvPswd = ""; // Empty the recvPswd variable
  } 
  
  else { //If the passwords doesn't match
    wrngPswdCounter += 1; // Increments the counter by 1
    if(wrngPswdCounter < 3) { //If the number of wrong attempts is less than 3 then another chance is provided.
      Serial.print("You have entered WRONG password. You have more ");
      Serial.print(3 - wrngPswdCounter);
      Serial.println(" time(s) to enter the correct one.");
      digitalWrite(RedLED, HIGH);
      delay(2000);
      digitalWrite(RedLED, LOW);
    } 
    
    else { //If the number of wrong attempts equals 3
      Serial.println("You have exceeded the number of attempts. Please reset the Arduino for re-entering the password or wait for 1 minute.");
      // Stops the whole process for 1 minute.
      digitalWrite(RedLED, HIGH);
      delay(60000);
      digitalWrite(RedLED, LOW);
      delay(1000);

      //Both the LEDs blink for 2 seconds indicating everything is reset.
      digitalWrite(RedLED, HIGH);
      digitalWrite(GreenLED, HIGH);
      delay(2000);
      digitalWrite(RedLED, LOW);
      digitalWrite(GreenLED, LOW);  
      Serial.println("Now you can try to enter the passwords.");  
      wrngPswdCounter = 0;  
   }
   recvPswd = ""; // Empty the recvPswd variable
    
  }
}

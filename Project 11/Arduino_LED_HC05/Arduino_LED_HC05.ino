int LED = 13; //Arduino Uno pin to which the LED's +ve pin is connected
String recvCommand = ""; //Stores the received command

void setup() {
  Serial.begin(9600); //Begin the Serial communication
  pinMode(LED, OUTPUT); //Set the direction of LED as OUTPUT
}

void loop() {
  if(Serial.available() > 0){ //Checks whether any information is available in Serial or not
    recvCommand = Serial.readString(); //Reads the string
    recvCommand.trim(); //Removes all the extra spaces at the beginning and at the end of the String.
  }

  if(recvCommand == "ON" || recvCommand == "on"){ //For switching ON the LED
    digitalWrite(LED, HIGH);
    Serial.println("LED is successfully switched ON.");
    recvCommand = "";
  }

  if(recvCommand == "OFF" || recvCommand == "off"){ //For switching OFF the LED
    digitalWrite(LED, LOW);
    Serial.println("LED is successfully switched OFF.");
    recvCommand = "";
  }
}

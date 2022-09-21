int pot = A5;     //Analog pin to which Potentiometer is connected.
int potVal = 0;

int LED = 6;      //PWM pin to which LED is connected.

void setup() {
  Serial.begin(9600);
  pinMode(pot, INPUT);  //Declare pot as INPUT port
  pinMode(LED, OUTPUT); //Set the LED port as OUTPUT.
}

void loop() {
  potVal = analogRead(pot);

  // Control the brightness of LED in steps.
  if(potVal <= 200) {
    Serial.println("Don't further decrease the resistance of the Potentiometer!!");
    analogWrite(LED, 0);
  } else if(potVal <= 400)
    analogWrite(LED, 75);
  else if(potVal <= 600)
    analogWrite(LED, 150);
  else if(potVal <= 800)
    analogWrite(LED, 200);
  else
    analogWrite(LED, 255);
}

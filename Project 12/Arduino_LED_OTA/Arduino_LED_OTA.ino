int LEDPin = 9; //Arduino Digital Pin 9

void setup() 
{
  Serial.begin(115200);     //Initialize the Arduino's Serial
  pinMode(LEDPin,OUTPUT);   //Set the direction of LEDpin
  digitalWrite(LEDPin,LOW); //Initially, keep the LED OFF
}

void loop() 
{
  digitalWrite(LEDPin,HIGH);  // Write HIGH to ON the LED
  Serial.println("LED is switched ON");
  delay(1000);                //Wait for a second
  
  digitalWrite(LEDPin,LOW);   // Write LOW to OFF the LED
  Serial.println("LED is switched OFF");
  delay(1000);                // Wait for a second
}

int FSRpin = A0; // Initialize the FSR pin
int FSRvalue; // Variable to store the FSR value

void setup() {
  Serial.begin(9600); //Start the Serial communication
  pinMode(FSRpin,INPUT); //Set the mode of FSR as INPUT
}

void loop() {
  FSRvalue = analogRead(FSRpin); //Read the FSR values
  Serial.print("FSR Value: "); //Print the FSR values
  Serial.println(FSRvalue);

  // Interpret the FSR values
  if(FSRvalue == 0)
    Serial.println("There is no pressure. \n");
  else if(FSRvalue < 200)
    Serial.println("Very little force is exerted on FSR. It can be considered as a soft touch. \n");
  else if(FSRvalue < 400)
    Serial.println("Little force is exerted on FSR. It can be considered as a hard touch. \n");
  else if(FSRvalue < 600)
    Serial.println("High force is exerted on FSR. It can be considered as a slight press. \n");
  else if(FSRvalue < 800)
    Serial.println("Very High force is exerted on FSR. It can be considered as a little press. \n");
  else
    Serial.println("Maximum force is exerted on FSR. It can be considered as a hard press. \n");
  
  delay(500);
}

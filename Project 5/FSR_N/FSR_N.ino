int FSRpin = A0;  //Initialize the FSR pin
int FSRvalue;     //Variable to store FSR analog values

int voltage;     //Variable to store FSR values to Voltage
unsigned long resistance;  //Variable to store the resistance calculated from the Voltage
unsigned long conductance; //Variable to store the conductance calculated from the Resistance
unsigned long force;      //Variable to store the force on FSR calculated from the Conductance
 
void setup() {
  Serial.begin(9600);     //Initialize the Arduino's serial
  pinMode(FSRpin, INPUT); //Set the FSR mode as INPUT
}
 
void loop() {
  FSRvalue = analogRead(FSRpin); //Read the FSR values
  Serial.print("FSR Value: "); //Print the FSR values
  Serial.println(FSRvalue);
 
  //Analog FSR value is converted to Voltage
  voltage = map(FSRvalue, 0, 1023, 0, 5000); 
  Serial.print("FSR Voltage (in mV): "); //Print voltage
  Serial.println(voltage);

  if (voltage == 0) { // If voltage is 0 then No pressure on FSR
    Serial.println("No Pressure (0N)");  
  } else {
    //Calculate the resistance from the non zero voltage
    resistance = (5000 - voltage);
    resistance *= 10000; 
    resistance /= voltage;
    Serial.print("FSR Resistance (in ohms): "); //Print resistance in Ohms
    Serial.println(resistance);
 
    conductance = 1000000/resistance; //Calculate conductance in microMho 
 
    if (conductance <= 1000) { //If conductance is less than 1000 (Low force Range)
      force = conductance / 80;
      Serial.print("Force (in Newtons): "); //Print force in Newton
      Serial.println(force);      
    } else { //If conductance is greater than 1000 (High force Range)
      force = (conductance - 1000)/30;
      Serial.print("Force (in Newtons): "); //Print force in Newton
      Serial.println(force);            
    }
  }
  Serial.println("");
  delay(1000);
}

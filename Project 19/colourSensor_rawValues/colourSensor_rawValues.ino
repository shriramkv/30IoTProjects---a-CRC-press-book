//Used for controlling colour sensor
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define out 7
#define nSampleAvg 10

int freqRed = 0;
int freqGreen = 0;
int freqBlue = 0;

void setup() {
  Serial.begin(9600); //Initialisation of Arduino's Serial

  //Direction of colour sensor pins
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(out, INPUT);
  
  //Frequency scaling of colour sensor set to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
}

void loop() {
    for(int i=0; i < nSampleAvg; i++){
    //Reading the output frequency of RED filtered photodiodes
    digitalWrite(S2,LOW);
    digitalWrite(S3,LOW);
    freqRed += pulseIn(out, LOW);
    delay(100);
    
    //Reading the output frequency of GREEN filtered photodiodes
    digitalWrite(S2,HIGH);
    digitalWrite(S3,HIGH);
    freqGreen += pulseIn(out, LOW);
    delay(100);
    
    //Reading the output frequency of BLUE filtered photodiodes
    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    freqBlue += pulseIn(out, LOW);
    delay(100);
  }

  Serial.print("R= ");
  Serial.print(freqRed/nSampleAvg); //Printing RED colour frequency
  Serial.print("  G= ");
  Serial.print(freqGreen/nSampleAvg); //Printing GREEN colour frequency
  Serial.print("  B= ");
  Serial.println(freqBlue/nSampleAvg); //Printing BLUE colour frequency

  freqRed = 0;
  freqGreen = 0;
  freqBlue = 0;
}

int LEDPin = 9;        //The PWM pin to which LED is attached.
int brightness = 0;   //Initially LED brightness.
int fadeAmount = 5;   //Quantity of LED fade per step

void setup() {
  pinMode(LEDPin, OUTPUT); //Set the LED port as OUTPUT port.
}

void loop() {
  // Set the brightness of LEDPin:
  analogWrite(LEDPin, brightness);

  // Change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // Reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }

  delay(50);  // wait for 50ms to see the dimming effect per step
}

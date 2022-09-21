# Import required libraries
import RPi.GPIO as GPIO
import time

# Used for numbering the pins according to GPIO pin number
GPIO.setmode(GPIO.BCM)
LEDPin = 12
# Used for numbering the pins according to the physical pin number
#GPIO.setmode(GPIO.BOARD)
#LEDPin = 32

# Used for setting the direction of operation of a pin.
GPIO.setup(LEDPin, GPIO.OUT)

# Set the frequency of PWM pin.
# Frequency -> 500
LED = GPIO.PWM(LEDPin, 500)
dutyCycle = 0
fadeAmount = 5
LED.start(dutyCycle) # Initially the duty cycle is 0

try:
    # Brightness of LED increases and then decreases
    while(1):
        dutyCycle = dutyCycle + fadeAmount
        LED.ChangeDutyCycle(dutyCycle)
        
        # Change the direction of fadeAmount
        if (dutyCycle <= 0 or dutyCycle >= 100):
            fadeAmount = -fadeAmount

        time.sleep(0.1) # Pauses for 100ms
      
except KeyboardInterrupt:  
    print("Brightness Control code is stopped.")  
  
finally:
    # This helps in switching off all the active channels and ensures a clean exit
    LED.stop()
    GPIO.cleanup()   

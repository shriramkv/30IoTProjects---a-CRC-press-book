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

try:
    while(1):
        # Used for writing digital voltage on a pin.
        GPIO.output(LEDPin, True)
        # Used for pausing the program.
        time.sleep(1)
        GPIO.output(LEDPin, False)
        time.sleep(1)

except KeyboardInterrupt:
    print("Blink code is stopped.")

finally:
    # This helps in switching off all the active channels and ensures a clean exit
    GPIO.cleanup()

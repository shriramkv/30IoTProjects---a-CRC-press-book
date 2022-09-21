# Import required libraries
import cv2
import RPi.GPIO as GPIO
import numpy as np
import time

# Create an empty LBPH face recognizer model
faceRecognizer = cv2.face.LBPHFaceRecognizer_create()
faceRecognizer.read('model/trainedModel.yml') # Imports the trained model
# Create a frontal detection classifier using haar cascade
faceCascade = cv2.CascadeClassifier("haarcascade_frontalface.xml")
font = cv2.FONT_HERSHEY_SIMPLEX # Font for writing on the Image

# Used for numbering the pins according to GPIO pin number
GPIO.setmode(GPIO.BCM)
servoPin = 18
# Used for numbering the pins according to the physical pin number
#GPIO.setmode(GPIO.BOARD)
#servoPin = 12

# Used for setting the direction of operation of a pin.
GPIO.setup(servoPin, GPIO.OUT)

# Set the frequency of PWM pin.
# Frequency -> 50
servo = GPIO.PWM(servoPin,50)
servo.start(0) # Initially the duty cycle is set to 0
# Time for which door must be open
timeDoorOpen = 20 

def doorControl():
    dutyCycle = 2

    # Rotate the servo from 0° to 180°
    print("Opening the Door")
    while (dutyCycle <= 12):
        servo.ChangeDutyCycle(dutyCycle)
        time.sleep(1)
        dutyCycle += 1

    servo.ChangeDutyCycle(0) # Keeps the servo still
    time.sleep(timeDoorOpen)

    # Rotate the servo from 180° to 0°
    print("Closing the Door")
    while (dutyCycle >= 2):
        servo.ChangeDutyCycle(dutyCycle)
        time.sleep(1)
        dutyCycle -= 1

    servo.ChangeDutyCycle(0) # Keeps the servo still

while (True):
    # Detect a face in the camera feed
    camera = cv2.VideoCapture(0)
    ret, cameraFeed = camera.read()
    grayCameraFeed = cv2.cvtColor(cameraFeed, cv2.COLOR_BGR2GRAY)
    faces = faceCascade.detectMultiScale(grayCameraFeed, 1.2, 5)

    for (x, y, w, h) in faces:
        # Draw rectangle around the detected face
        cv2.rectangle(cameraFeed, (x, y), (x + w, y + h), (0, 260, 0), 7)
        # Recognize the face
        ID, conf = faceRecognizer.predict(grayCameraFeed[y:y+h, x:x+w])
        
        if(ID == 1):
            # Write text on the image frame
            # Replace ID_1 with the name of 1st Person
            cv2.putText(cameraFeed, "ID_1", (x, y-40), font, 1, (0, 255, 255), 2)
            doorControl() # Opens the door

        if(ID == 2):
            # Write text on the image frame
            # Replace ID_2 with the name of 2nd Person
            cv2.putText(cameraFeed, "ID_2", (x, y-40), font, 1, (0, 255, 255), 2)
            print ("Entry Restricted!!")
            
    # Shows the camera feed     
    cv2.imshow("Face Recognizer", cameraFeed)
    camera.release() # Stops recording video from the camera
    ID = -1

    # If q is pressed then break from the loop
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Closes all the windows opened by OpenCV
cv2.destroyAllWindows()
# This helps in switching off all the active channels and ensures a clean exit
servo.stop()
GPIO.cleanup()

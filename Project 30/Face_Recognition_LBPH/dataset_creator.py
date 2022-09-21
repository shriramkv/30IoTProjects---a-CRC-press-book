# Import required libraries
import cv2
import numpy as np
import time

# Create a frontal detection classifier using haar cascade
faceCascade = cv2.CascadeClassifier('haarcascade_frontalface.xml')
ID = input("Please enter a unique ID: ")
# Starts recording video from the camera
camera = cv2.VideoCapture(0)
sampleIter = 0 
numSamples = 150 # Number of sample faces for training

while (True):
    # Returns image frames from the video
    ret, cameraFeed = camera.read()
    # Converts BGR frames to grayscale frames
    grayCameraFeed = cv2.cvtColor(cameraFeed, cv2.COLOR_BGR2GRAY)
    # Detect faces in the image frame
    faces = faceCascade.detectMultiScale(grayCameraFeed, 1.3, 5)

    for (x, y, w, h) in faces:
        # Draw a rectangle around the face
        cv2.rectangle(cameraFeed, (x, y), (x+w, y+h), (255, 0, 0), 2)
        # Store the face in the dataset folder
        cv2.imwrite("dataset/" + str(ID) + "." + str(sampleIter)+".jpg", grayCameraFeed[y:y+h, x:x+w])
        sampleIter += 1
        cv2.waitKey(25) # Waits for 25ms

    # Shows the frame  
    cv2.imshow("Face Detector", cameraFeed)

    # If required number of samples are collected break from the loop
    if (sampleIter == numSamples):
        print("Dataset with "+ str(numSamples) +" faces is successfully created.")
        break

    # If q is pressed then break from the loop
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Stops recording video from the camera 
camera.release()
cv2.destroyAllWindows() # Closes all the windows opened by OpenCV

# Import required libraries
import cv2
from PIL import Image
import numpy as np
import os

# Create an empty LBPH face recognizer model
model = cv2.face.LBPHFaceRecognizer_create()
# Folder path in which dataset is stored
datasetPath = 'dataset' 

# Stores complete path of the images 
imgPaths = [os.path.join(datasetPath,imgFile) for imgFile in os.listdir(datasetPath)]
faces = [] # Stores the facial images in the form of a numpy array
IDs = [] # Stores the respective IDs of the faces

# Iters through all the images
for imgPath in imgPaths:
    # Converts any BGR image to grayscale image
    faceImg = Image.open(imgPath).convert('L')
    # Converts facial image into numpy array
    faceArr = np.array(faceImg, 'uint8')
    faces.append(faceArr)
    # Obtains the ID of the Image from the Image file name
    ID = int(os.path.split(imgPath)[-1].split('.')[0])
    IDs.append(ID)
    # Shows each image
    cv2.imshow("Model Trainer", faceArr)
    cv2.waitKey(10) # Waits for 10ms

# Trains the model using the facial images array
model.train(faces, np.array(IDs))
model.save('model/trainedModel.yml') # Saves the trained model in model folder
print("Trained model is saved in model folder")
cv2.destroyAllWindows() # Closes all the windows opened by OpenCV

import cv2
import numpy as np
from PIL import Image   
      
def get_pixel(img, center, x, y):
    
    newPixelValue = 0
    try:
        # Neighbourhood value is greater than or equal to center
        if img[x][y] >= center:
            newPixelValue = 1
              
    except: 
        # No neighbourhood value (At boundaries)
        pass
      
    return newPixelValue
   
# LBP value calculation
def lbp_calculated_pixel(img, x, y):
   
    center = img[x][y]
    binaryVal = []
      
    # Top Right
    binaryVal.append(get_pixel(img, center, x-1, y+1))
      
    # Right
    binaryVal.append(get_pixel(img, center, x, y+1))
      
    # Bottom Right
    binaryVal.append(get_pixel(img, center, x+1, y+1))
      
    # Bottom
    binaryVal.append(get_pixel(img, center, x+1, y))
      
    # Bottom Left
    binaryVal.append(get_pixel(img, center, x+1, y-1))
      
    # Left
    binaryVal.append(get_pixel(img, center, x, y-1))
      
    # Top Left
    binaryVal.append(get_pixel(img, center, x-1, y-1))
      
    # Top
    binaryVal.append(get_pixel(img, center, x-1, y))
       
    # Decimal to binary conversion
    power_val = [1, 2, 4, 8, 16, 32, 64, 128]
    decimalVal = 0
      
    for i in range(len(binaryVal)):
        decimalVal += binaryVal[i] * power_val[i]
          
    return decimalVal
   
path = r"C:\Users\Anudeep\Desktop\Input.png"
inp_img = cv2.imread(path, 1)
 
# Height and Width of input image
height, width, _ = inp_img.shape
   
# RGB to gray scale conversion
gray_inp_img = cv2.cvtColor(inp_img, cv2.COLOR_BGR2GRAY)
   
# Numpy array of Input image size 
img_lbp_arr = np.zeros((height, width), np.uint8)
   
for i in range(0, height):
    for j in range(0, width):
        img_lbp_arr[i, j] = lbp_calculated_pixel(gray_inp_img, i, j)
  
LBP_img = Image.fromarray(img_lbp_arr)
LBP_img.save(r"C:\Users\Anudeep\Desktop\LBP.png")
print("LBP image is saved successfully")
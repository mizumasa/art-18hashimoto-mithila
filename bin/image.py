## License: Apache 2.0. See LICENSE file in root directory.
## Copyright(c) 2015-2017 Intel Corporation. All Rights Reserved.

###############################################
##      Open CV and Numpy integration        ##
###############################################
import sys
import numpy as np
import cv2
from PIL import Image

def main(filename):
    print filename
    img = Image.open(filename)
    imga = np.asarray(img)
    h,w,d = imga.shape
    outa = np.ones((h,h,d),dtype="uint8") * 255
    outa[:,int((h-w)/2):int((h-w)/2)+w,:] = imga
    Image.fromarray(outa).save(filename.replace(".png",".jpg"))
    pass

if __name__=='__main__':
	argvs=sys.argv
	print argvs
	main(argvs[1])

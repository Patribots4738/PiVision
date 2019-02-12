#!/usr/bin/env python3
#
# Demonstrates streaming and modifying the image via OpenCV
#


import cscore as cs
import numpy as np
import time
import cv2


def main():
    playerCam = cs.UsbCamera("player cam", 0)
    playerCam.setVideoMode(cs.VideoMode.PixelFormat.kMJPEG, 320, 240, 30)

    mjpegServer = cs.MjpegServer("httpserver", 8081)
    mjpegServer.setSource(playerCam)

    cvsink = cs.CvSink("cvsink_p")
    cvsink.setSource(playerCam)

    print("mjpg server listening at http://0.0.0.0:8081")

    cam = cs.UsbCamera("vision cam", 1)
    cam.setVideoMode(cs.VideoMode.PixelFormat.kMJPEG, 320, 240, 30)

    #Remove me on final code, leaving it in for testing
    mjpegServer_cv = cs.MjpegServer("httpserver", 8082)
    mjpegServer_cv.setSource(cam)
    #------------------------------------------------

    cvsink = cs.CvSink("cvsink")
    cvsink.setSource(cam)

    cvSource = cs.CvSource("cvsource", cs.VideoMode.PixelFormat.kMJPEG, 320, 240, 30)
    cvMjpegServer = cs.MjpegServer("cvhttpserver", 8083)
    cvMjpegServer.setSource(cvSource)

    print("OpenCV output mjpg server listening at http://0.0.0.0:8082")

    src = np.zeros(shape=(240, 320, 3), dtype=np.uint8)
    dst = np.zeros(shape=(240, 320, 3), dtype=np.uint8)

    up = np.array([255, 255, 255])
    low = np.array([0, 0, 0])


    while True:

        time, src = cvsink.grabFrame(src)
        if time == 0:
            print("error:", cvsink.getError())
            continue
	#put processing code in here
	#print("got frame at time", time, src.shape)
	dst = cv2.flip(src, 1)
	dst = cv2.cvtColor(dst, cv2.COLOR_BGR2HSV)
	mask = cv2.inRange(dst, low, up)
	
	mask, contours, heirarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
	if contours.Count() <= 0:
	    continue

	cnt = contours[0]
	M = cv2.moments(cnt)
	
	cx = int(M['m10']/M['m00'])
	cy = int(M['m01']/M['m00'])
        
	hull = cv2.convexHull(cnt)

	rect = cv2.minAreaRect(cnt)
    	box = cv2.boxPoints(rect)
    	box = np.int0(box)
    	cv2.drawContours(img,[box],0,(0,0,255),2)
	print("detected angle : " + str(box.angle))
	
        #Not after here
        cvSource.putFrame(src)


if __name__ == "__main__":
    main()

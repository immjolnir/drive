#!/usr/bin/env python3
# encoding: utf-8

import sys
import cv2
import numpy as np

from viz import color

#通过 image.shape 获取图像的宽、高、通道数；
#通过 image.dtype 获取图像数据类型；
#通过 image.size 获取图像的像素总数。
#
class Canvas:
    def __init__(self, width, height, padding=True):
        self._width = width
        self._height = height

        if padding:
            self.image = np.zeros((height+10, width+10 , 3), np.uint8)
            self.image = cv2.line(self.image, (0, height), (width, height), color.white, thickness = 1)
        else:
            self.image = np.zeros((height, width, 3), np.uint8)


    def xline(self, x):
        self.image = cv2.line(self.image, (x, 0), (x, self._height), color.white, thickness = 1)

    def yline(self, y):
        self.image = cv2.line(self.image, (0, y), (self._width, y), color.white, thickness = 1)

    def circle(self, x, y, radius=0, color=color.red, thickness=-1):
        self.image = cv2.circle(self.image, (x, y), radius=radius, color=color, thickness=thickness)

    def info(self):
        print(self.image.shape)
        print(self.image.dtype)
        print(self.image.size)

    def show(self, name):
        cv2.imshow(name, self.image)
        cv2.setWindowProperty(name, cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)

        # start the event loop - essential
        # cv2.waitKey() is a keyboard binding function (argument is the time in
        # ms). It waits for specified milliseconds for any keyboard event.
        # If you press any key in that time, the program continues.
        # If 0 is passed, it waits indefinitely for a key stroke.
        # (bitwise and with 0xFF to extract least significant byte of
        # multi-byte response)

        # key = cv2.waitKey(40) & 0xFF
        # wait 40ms (i.e. 1000ms / 25 fps = 40 ms)
        # It can also be set to detect specific key strokes by recording which key is pressed

        # e.g. if user presses "x" then exit
        # e.g. if user presses "x" then exit  / press "f" for fullscreen display
        key = cv2.waitKey()

        cv2.destroyAllWindows()
        if (key == ord('x')):
            sys.exit()

    def save(self, name):
        cv2.imwrite(name, self.image)

if __name__ == '__main__':
    width = 960
    height = 540

    canvas = Canvas(width, height)
    canvas.info()

    # Anchor
    canvas.circle(5, 5, radius=2, color=color.green)
    canvas.circle(width, height, radius=2, color=color.green)
    canvas.show("test")



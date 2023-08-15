#!/usr/bin/env python3
# encoding: utf-8

import cv2
import sys
import numpy as np

from viz import color

class Transformer(object):
    def __init__(self, scale):
        self._k = scale
        self._R = np.array([[0, -1], [-1, 0]])
        self._T = np.zeros([2, 1])

    @property
    def T(self):
        return self._T

    @T.setter
    def T(self, translation):
        self._T = translation

    def transform(self, p):
        return self._k * (self._R.dot(p)) + self._T

class Canvas:
    def __init__(self, scale, min_x=-160, max_x=310, min_y=-40, max_y=40, step=5):
        self._transformer = Transformer(scale)
        self._create(min_x, max_x, min_y, max_y, step)

    @property
    def transformer(self):
        return self._transformer

    @property
    def ortho_image(self):
        return self._ortho_image

    def circle(self, x, y, radius=0, color=color.red, thickness=-1):
        _x, _y = self.project_point(x, y)
        self._ortho_image = cv2.circle(self._ortho_image, (_x, _y), radius=radius, color=color, thickness=thickness)

    #cv2.line(self._ortho_image, pt3, pt4, line_color, line_thickness)

    def project_point(self, x, y):
        def toTuple(pt):
            return (int(round(pt[0][0])), int(round(pt[1][0])))
        newp = self._transformer.transform(np.array((x, y)).reshape(2, 1))
        return toTuple(newp)


    def _create(self, min_x, max_x, min_y, max_y, step):
        top_left = (max_x, max_y)
        bottom_right = (min_x, min_y)

        # get the points on image
        pt1 = self._transformer.transform(np.array(top_left).reshape(2, 1))
        pt2 = self._transformer.transform(np.array(bottom_right).reshape(2, 1))

        self._transformer.T = -pt1

        image_range = pt2 - pt1
        width, height = int(round(image_range[0][0])), int(round(image_range[1][0]))
        self._ortho_image = np.zeros((height, width, 3), dtype="uint8")
        self._ortho_image[:] = color.black

        for imu_x in range(min_x, max_x, step):
            imu_line_color = color.dark_gray
            if not imu_x % 50:
                imu_line_color = color.light_gray

            image_pt1 = self.project_point(imu_x, min_y)
            image_pt2 = self.project_point(imu_x, max_y)
            cv2.line(self._ortho_image, image_pt1, image_pt2, imu_line_color)

        for imu_y in range(min_y, max_y, step):
            image_pt1 = self.project_point(min_x, imu_y)
            image_pt2 = self.project_point(max_x, imu_y)
            cv2.line(self._ortho_image, image_pt1, image_pt2, color.dark_gray)

        self._drawEgo()

    def _drawEgo(self):
        ego_imu_x = 0
        ego_imu_y = 0
        pt = self.project_point(ego_imu_x, ego_imu_y)
        cv2.circle(self._ortho_image, pt, 5, color.red, 2)

    def show(self, name):
        cv2.imshow(name, self._ortho_image)
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
        cv2.imwrite(name, self._ortho_image)


if __name__ == '__main__':
    canvas = Canvas(3)
    canvas.circle(10, 10, radius=2)
    canvas.show("test")


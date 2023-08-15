#!/usr/bin/env python3
# encoding: utf-8


import cv2

from viz import color
import numpy as np
import imu


class Canvas:
    def __init__(self, width, height, scale):
        self.image = np.zeros((height, width, 3), np.uint8)
        self.image[:] = color.gray

        self._width = width
        self._height = height
        self._scale = scale

        self._image_ratio_x = None
        self._image_ratio_y = None

        self._xscale_ratio = 1
        self._yscale_ratio = 1

        self._image_offset_x = 10
        self._image_offset_y = int(self._height/2)

        self._original_phisical_x = None
        self._original_phisical_y = None

        self._original_phisical_xs = []
        self._original_phisical_ys = []

    @property
    def width(self):
        return self._width

    @property
    def height(self):
        return self._height

    @property
    def scale(self):
        return self._scale

    def get_image_x(self, x):
        if not self._original_phisical_x:
            self._original_phisical_x = x

        return self._image_offset_x + int((x - self._original_phisical_x) * self._xscale_ratio)

    def get_image_y(self, y):
        if not self._original_phisical_y:
            self._original_phisical_y = y

        return self._image_offset_y + int((y - self._original_phisical_y) * self._yscale_ratio)

    def get_image_point(self, x, y):
        return (self.get_image_x(x), self.get_image_y(y))

    def circle(self, x, y, text=None, radius=0, color=color.red, thickness=-1):
        if len(self._original_phisical_xs) < 2 or len(self._original_phisical_ys) < 2:
            self._original_phisical_xs.append(x)
            self._original_phisical_ys.append(y)
            return

        if len(self._original_phisical_xs) == 2 and len(self._original_phisical_ys) == 2:
            # break the if-condition.
            self._original_phisical_xs.append(x)
            self._original_phisical_ys.append(y)
            # reset scale
            self._xscale_ratio = self.scale / (self._original_phisical_xs[1] - self._original_phisical_xs[0])
            self._yscale_ratio = self.scale / (self._original_phisical_ys[1] - self._original_phisical_ys[0])

            self.circle(self._original_phisical_xs[0], self._original_phisical_ys[0], text, radius, color, thickness)
            self.circle(self._original_phisical_xs[1], self._original_phisical_ys[1], text, radius, color, thickness)


        image_x, image_y = self.get_image_point(x, y)

        point = (image_x, image_y)
        if image_x < self._width and image_y < self._height:
            self.image = cv2.circle(self.image, point, radius=radius, color=color, thickness=thickness)
            if text:
                cv2.putText(self.image, text, point, cv2.FONT_HERSHEY_SIMPLEX, 1, color, 2, cv2.LINE_AA)
        else:
            print("Point(x={}, y={}) is out of the image".format(image_x, image_y))
            print(self._xscale_ratio)
            print(self._yscale_ratio)


    def xline(self, x):
        image_x = self.get_image_x(x)
        self.image = cv2.line(self.image, (image_x, 0), (image_x, self._height), color.white, thickness = 1)

    def yline(self, y):
        image_y = self.get_image_y(y)
        self.image = cv2.line(self.image, (0, image_y), (self._width, image_y), color.white, thickness = 1)


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

def draw_yaw(radar_pose, all_poses):
    width = 960
    height = 540
    scale = 8
    canvas = Canvas(width, height, scale)

    for index, row in all_poses.iterrows():
        #print("{},{}, yaw={}, yaw_rate={}".format(row.x, row.y, row.yaw, row.yaw_rate))
        #text = "{}, {}".format(row.yaw, row.yaw_rate)
        canvas.circle(row.time, row.yaw, radius=2, color=color.white)

    radar_text = "{}, {}".format(radar_pose.time, radar_pose.yaw)
    canvas.circle(radar_pose.time, radar_pose.yaw, radar_text, radius=2, color=color.green)

    canvas.save("pose-yaw.png")

def draw_yaw_rate(radar_pose, all_poses):
    width = 960
    height = 540
    scale = 8
    canvas = Canvas(width, height, scale)

    for index, row in all_poses.iterrows():
        canvas.circle(row.time, row.yaw_rate, radius=2, color=color.white)

    radar_text = "{}, {}".format(radar_pose.time, radar_pose.yaw_rate)
    canvas.circle(radar_pose.time, radar_pose.yaw_rate, radar_text, radius=2, color=color.green)

    canvas.save("pose-yaw_rate.png")

def draw_position(radar_pose, all_poses):
    width = 960
    height = 540
    scale = 6
    canvas = Canvas(width, height, scale)

    for index, row in all_poses.iterrows():
        #print("{},{}, yaw={}, yaw_rate={}".format(row.x, row.y, row.yaw, row.yaw_rate))
        text = "{}, {}".format(row.yaw, row.yaw_rate)
        canvas.circle(row.x, row.y, radius=2, color=color.white)

    radar_text = "{}, {}".format(radar_pose.yaw, radar_pose.yaw_rate)
    canvas.circle(radar_pose.x, radar_pose.y, radar_text, radius=2, color=color.green)

    #canvas.save("all_poses.png")
    canvas.show("all_poses.png")


def main(imu_pose_trajectory):

    stereo_pose = imu_pose_trajectory.iloc[0]
    radar_pose = imu_pose_trajectory.iloc[1]
    pose_trajectories = imu_pose_trajectory.iloc[2:]

    #draw_position(radar_pose, pose_trajectories)
    draw_yaw(radar_pose, pose_trajectories)
    draw_yaw_rate(radar_pose, pose_trajectories)


if __name__ == '__main__':
    smoothed_file = 'smooth_yaw_rate/pose_trajectory-62.csv'
    smoothed_imu_pose_trajectory = imu.IMUPoseTrajectory.load(smoothed_file)

    nosmoothed_file = 'no-smooth_yaw_rate/pose_trajectory-62.csv'
    nosmoothed_imu_pose_trajectory = imu.IMUPoseTrajectory.load(nosmoothed_file)

    #main(smoothed_imu_pose_trajectory)
    main(nosmoothed_imu_pose_trajectory)

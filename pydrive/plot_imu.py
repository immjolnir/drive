#!/usr/bin/env python3
# encoding: utf-8

# Import required packages:
import cv2
import numpy as np
import matplotlib.pyplot as plt
import math
import pandas as pd
import munch

from viz import color

#通过 image.shape 获取图像的宽、高、通道数；
#通过 image.dtype 获取图像数据类型；
#通过 image.size 获取图像的像素总数。
#
class IMUPoseTrajectory:
    def __init__(self, h, w):
        self.image = np.zeros((h+50, w+50, 3), np.uint8)

    def draw_point(self, x, y, radius=0, color=(0, 0, 255), thickness=-1):
        self.image = cv2.circle(self.image, (x, y), radius=radius, color=color, thickness=thickness)

    def show(self, name):
        cv2.imshow(name, self.image)
        cv2.waitKey(0)
        cv2.destroyAllWindows()

class Coo:
    def __init__(
        self, lower_timestamp, upper_timestamp, lower_yaw, upper_yaw, width, height
    ):
        self.lower_timestamp, self.upper_timestamp, self.lower_yaw, self.upper_yaw = (
            lower_timestamp,
            upper_timestamp,
            lower_yaw,
            upper_yaw,
        )
        self.image_width = width
        self.image_height = height

        self.timestamp_range = self.upper_timestamp - self.lower_timestamp

        self.yaw_range = self.upper_yaw - self.lower_yaw

    def get_x(self, timestamp):
        return (
            (timestamp - self.lower_timestamp)
            * 1.0
            / self.timestamp_range
            * self.image_width
        )

    def get_y(self, yaw):
        return (yaw - self.lower_yaw) * 1.0 / self.yaw_range * self.image_height

def load_csv(file):
    # Load the data
    imu_pose_trajectory = pd.read_csv(file,
        sep=",",
        header=None,
        names=[
            "timestamp",
            "x",
            "y",
            "z",
            "vx",
            "vy",
            "roll",
            "pitch",
            "yaw",
            "roll_rate",
            "pitch_rate",
            "yaw_rate",
        ],
    )
    return imu_pose_trajectory


def nosmooth(file):
    imu_pose_trajectory = load_csv(file)

    stereo_pose = imu_pose_trajectory.iloc[0]
    radar_pose = imu_pose_trajectory.iloc[1]
    pose_trajectories = imu_pose_trajectory.iloc[2:]


    poses = {}  # timestmap => yaw
    for index, row in pose_trajectories.iterrows():
        poses[row.timestamp] = row.yaw_rate

    timestamps = sorted(poses.keys())
    yaws = sorted(poses.values())
    lower_timestamp, upper_timestamp = timestamps[0], timestamps[-1]
    lower_yaw, upper_yaw = yaws[0], yaws[-1]

    return lower_timestamp,upper_timestamp, lower_yaw, upper_yaw, stereo_pose, radar_pose, poses


def main():
    width = 960
    height = 540
    # Create an image with size: 540 * 960
    canvas = IMUPoseTrajectory(height, width)

    lower_timestamp,upper_timestamp, lower_yaw, upper_yaw, stereo_pose, radar_pose, poses = nosmooth("no-smooth_yaw_rate/pose_trajectory-62.csv")

    coo = Coo(lower_timestamp, upper_timestamp, lower_yaw, upper_yaw, width, height)
    print(coo.get_x(lower_timestamp), coo.get_x(upper_timestamp))
    print(coo.get_y(lower_yaw), coo.get_y(upper_yaw))

    # Anchor
    canvas.draw_point(5, 5, radius=2, color=(0,255,0))
    canvas.draw_point(940, 530,radius=2, color=(0,255,0))
    for timestamp, yaw in poses.items():
        x = coo.get_x(timestamp)
        y = coo.get_y(yaw)
        canvas.draw_point(int(x), int(y), radius=2)

    canvas.draw_point(int(coo.get_x(stereo_pose.timestamp)), int(coo.get_y(stereo_pose.yaw_rate)), radius=3, color=(255, 0, 0))
    canvas.draw_point(int(coo.get_x(radar_pose.timestamp)), int(coo.get_y(radar_pose.yaw_rate)), radius=3, color=(255,255,0))

    # Draw smoothed
    _, _, _, _, stereo_pose, radar_pose, poses = nosmooth("smooth_yaw_rate/pose_trajectory-62.csv")
    canvas.draw_point(int(coo.get_x(stereo_pose.timestamp)), int(coo.get_y(stereo_pose.yaw_rate)), radius=2, color=color.gold)
    canvas.draw_point(int(coo.get_x(radar_pose.timestamp)), int(coo.get_y(radar_pose.yaw_rate)), radius=2, color=color.yellow)
    for timestamp, yaw in poses.items():
        x = coo.get_x(timestamp)
        y = coo.get_y(yaw)
        canvas.draw_point(int(x), int(y), radius=2, color=(192, 192, 192))

    canvas.show("Pose")

if __name__ == '__main__':
    main()

#!/usr/bin/env python3
# encoding: utf-8

import numpy as np
import pandas as pd

class IMUPoseTrajectory:
    @classmethod
    def load(cls, file):
        # Load the data
        pose_trajectory = pd.read_csv(file,
            sep=",",
            header=None,
            names=[
                "time",
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
        return pose_trajectory

def main():
    nosmoothed_file = 'no-smooth_yaw_rate/pose_trajectory-62.csv'

    imu_pose_trajectory = IMUPoseTrajectory.load(nosmoothed_file)

    stereo_pose = imu_pose_trajectory.iloc[0]
    radar_pose = imu_pose_trajectory.iloc[1]
    pose_trajectories = imu_pose_trajectory.iloc[2:]

    print(stereo_pose)
    print(radar_pose)

    for index, row in pose_trajectories.iterrows():
        print(index)
        print(row)
        break

if __name__ == '__main__':
    main()

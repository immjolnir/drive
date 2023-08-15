#!/usr/bin/env python3
# encoding: utf-8

import pandas as pd

from viz import color
from coo import Coo
from scaled_canvas import Canvas
from point import Point


def load_radar_tracks(file):
    df = pd.read_csv(file, sep=" ", header=None, names=["time", "track_id", "polygon", "vx", "vy", "vz"])
    return df

class RadarTracklet:
    def __init__(self, track_time, track_id, vx, vy):
        self._track_time = track_time
        self._track_id = track_id
        self._polygon = []
        self._vx = vx
        self._vy = vy

    def add_polygon(self, polygon):
        positions = polygon.split(";")
        for p in positions:
            xyz = p.split(",")
            if len(xyz) == 3:
                self._polygon.append(Point(float(xyz[0]), float(xyz[1]), float(xyz[2])))
            else:
                print("Wrong poistion")

    @property
    def track_time(self):
        return self._track_time

    @property
    def track_id(self):
        return self._track_id

    @property
    def polygon(self):
        return self._polygon

    @property
    def vx(self):
        return self._vx

    @property
    def vy(self):
        return self._vy

    def __repr__(self):
        return "RadarTracklet(time:{}, id:{}, polygon:{}, vx:{}, vy:{})".format(self._track_time, self._track_id, self._polygon, self._vx, self._vy)

if __name__ == '__main__':
    df = load_radar_tracks("radar-detections.txt")

    for index, row in df.iterrows():
        track_time = row.time
        track_id = row.track_id
        polygon = row.polygon
        tracklet = RadarTracklet(track_time, track_id, row.vx, row.vy)
        tracklet.add_polygon(polygon)
        print(tracklet)

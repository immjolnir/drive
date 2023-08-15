#!/usr/bin/env python3
# encoding: utf-8

import pandas as pd

from viz import color
from coo import Coo
from scaled_canvas import Canvas
import radar_tracklet


def plot_tracklets(tracks):
    # frame: time -> {tracklet_id, [points]}
    for track_time, tracklets in tracks.items():
        # Process a frame one time.
        frame_canvas = Canvas(3)
        print("="*20)
        if track_time not in [1677136322162, 1677136322242, 1677136322313, 1677136322382, 1677136322462]:
            continue

        for tracklet in tracklets:
            tracklet_color = color.get_color(tracklet.track_id)
            # polygon 怎么是重复的?
            # [(114.800003, -0.6, 0.0), (114.800003, -0.6, 0.0), (114.800003, -0.6, 0.0), (114.800003, -0.6, 0.0)]
            #print("time: {}, vx={}, vy={}, positions={}".format(tracklet.track_time, tracklet.vx, tracklet.vy, tracklet.polygon))
            printable = False
            for p in tracklet.polygon:
                if p.y > -2 and p.y < 2:
                    printable = True
                    frame_canvas.circle(p.x, p.y, radius=2, color=tracklet_color, thickness=2)
            if printable:
                print(tracklet)
        #frame_canvas.show("track: {}".format(track_time))
        frame_canvas.save("track_{}.png".format(track_time))

def main():
    df = radar_tracklet.load_radar_tracks("radar-detections.txt")

    tracks = {}
    for index, row in df.iterrows():
        track_time = row.time
        track_id = row.track_id
        polygon = row.polygon

        tracklet = radar_tracklet.RadarTracklet(track_time, track_id, row.vx, row.vy)
        tracklet.add_polygon(polygon)

        if track_time not in tracks:
            tracks[track_time] = []
        tracks[track_time].append(tracklet)

    plot_tracklets(tracks)



if __name__ == '__main__':
    main()



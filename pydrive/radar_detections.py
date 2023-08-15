import pandas as pd

from canvas import Canvas
from viz import color
from coo import Coo


def load_radar_tracks(file):
    df = pd.read_csv(file, sep=" ", header=None, names=["time", "track_id", "polygon", "vx", "vy", "vz"])
    return df

class RadarTracklet:
    def __init__(self, track_time, track_id):
        self._track_time = track_time
        self._track_id = track_id
        self._polygon = []

    def add_position(self, x, y, z):
        self._polygon.append((x,y,z))

    @property
    def track_time(self):
        return self._track_time

    @property
    def track_id(self):
        return self._track_id

    @property
    def polygon(self):
        return self._polygon


def plot_tracklets(tracks):
    # frame: time -> {tracklet_id, [points]}
    width = 540
    height = 960


    for track_time, tracklets in tracks.items():
        # Process a frame one time.
        frame_canvas = Canvas(width, height)
        xs = []
        ys = []
        for tracklet in tracklets:
            for x, y, z in tracklet.polygon:
                #print("({},{},{})".format(x, y, z))
                xs.append(x)
                ys.append(y)

        assert len(xs) == len(ys)
        xs = sorted(xs)
        ys = sorted(ys)
        coo_x = Coo(xs[0], xs[-1], width)
        coo_y = Coo(ys[0], ys[-1], height)

        # 0, 0 is not in the range
        #frame_canvas.circle(coo_x.convert(0), coo_y.convert(0), radius=2, color=color.green, thickness=5)
        frame_canvas.xline(coo_x.convert(0))
        frame_canvas.yline(coo_y.convert(0))

        # Start the plotting
        for tracklet in tracklets:
            tracklet_color = color.get_color(tracklet.track_id)
            for _x, _y, _ in tracklet.polygon:
                x = coo_x.convert(_x)
                y = coo_y.convert(_y)
                frame_canvas.circle(x, y, radius=2, color=tracklet_color, thickness=2)
        #frame_canvas.show("track: {}".format(track_time))
        frame_canvas.save("track_{}.png".format(track_time))

def main():
    df = load_radar_tracks("radar-detections.txt")

    tracks = {}
    for index, row in df.iterrows():
        track_time = row.time
        track_id = row.track_id
        polygon = row.polygon

        tracklet = RadarTracklet(track_time, track_id)

        positions = polygon.split(";")
        #print(positions)
        if not len(positions):
            print("empty line")
            continue
        for p in positions:
            xyz = p.split(",")
            if len(xyz) == 3:
                tracklet.add_position(float(xyz[0]), float(xyz[1]), float(xyz[2]))
            else:
                print("Wrong poistion")
                break

        if track_time not in tracks:
            tracks[track_time] = []
        tracks[track_time].append(tracklet)

    plot_tracklets(tracks)



if __name__ == '__main__':
    main()



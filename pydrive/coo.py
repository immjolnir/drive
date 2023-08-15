class Coo:
    def __init__(self, lower_boundary, upper_boundary, projected_range):
        self.lower_boundary, self.upper_boundary, self.projected_range = lower_boundary, upper_boundary, projected_range
        self.actual_range = self.upper_boundary  - self.lower_boundary

    def convert(self, x):
        return int( (x - self.lower_boundary) * 1.0 / self.actual_range * self.projected_range )


#
# A copy of opencv/modules/viz/include/opencv2/viz/types.hpp
# They are defined as readonly properties here.
#
class Color:
    """
    This class a represents BGR color.
    """
    def get_color(self, id):
        red   = (id * 2453) % 128 + 96
        green = (id * 15645) % 128 + 96
        blue  = (id * 8764) % 128 + 96
        return (blue, green, red)

    @property
    def black(self):
        return (  0,   0,   0)

    @property
    def green(self):
        return (  0, 255,   0)

    @property
    def blue(self):
        return (255,   0,   0)

    @property
    def cyan(self):
        return (255, 255,   0)

    @property
    def red(self):
        return (  0,   0, 255)

    @property
    def yellow(self):
        return (  0, 255, 255)

    @property
    def magenta(self):
        return (255,   0, 255)

    @property
    def white(self):
        return (255, 255, 255)

    @property
    def gray(self):
        return (128, 128, 128)

    @property
    def dark_gray(self):
        return (50, 50, 50)

    @property
    def light_gray(self):
        return (220, 220, 220)

    @property
    def mlab(self):
        return (255, 128, 128)

    @property
    def navy(self):
        return (0,     0, 128)

    @property
    def olive(self):
        return (0,   128, 128)

    @property
    def maroon(self):
        return (0,     0, 128)

    @property
    def teal(self):
        return (128, 128,   0)

    @property
    def rose(self):
        return (128,   0, 255)

    @property
    def azure(self):
        return (255, 128,   0)

    @property
    def lime(self):
        return (0,   255, 191)

    @property
    def gold(self):
        return (0,   215, 255)

    @property
    def brown(self):
        return (0,    75, 150)

    @property
    def orange(self):
        return (0,   165, 255)

    @property
    def chartreuse(self):
        return (0,   255, 128)

    @property
    def orange_red(self):
        return (0,    69, 255)

    @property
    def purple(self):
        return (128,   0, 128)

    @property
    def indigo(self):
        return (130,   0,  75)

    @property
    def pink(self):
        return (203, 192, 255)

    @property
    def cherry(self):
        return ( 99,  29, 222)

    @property
    def bluberry(self):
        return (247, 134,  79)

    @property
    def raspberry(self):
        return ( 92,  11, 227)

    @property
    def silver(self):
        return (192, 192, 192)

    @property
    def violet(self):
        return (226,  43, 138)

    @property
    def apricot(self):
        return (177, 206, 251)

    @property
    def turquoise(self):
        return (208, 224,  64)

    @property
    def celestial_blue(self):
        return (208, 151,  73)

    @property
    def amethyst(self):
        return (204, 102, 153)

    @property
    def not_set(self):
        return (-1, -1, -1)

color = Color()

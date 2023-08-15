
class Point:
    def __init__(self, x, y, z):
        self._x = x
        self._y = y
        self._z = z

    # 不能重载构造方法吗？
    #def __init__(self, triple):
    #    self._x = triple[0]
    #    self._y = triple[1]
    #    self._z = triple[2]

    def __repr__(self):
        return "Point(x:{}, y:{}, z:{})".format(self._x, self._y, self._z)

    @property
    def x(self):
        return self._x

    @property
    def y(self):
        return self._y

    @property
    def z(self):
        return self._z

# If you want something you've never had, then you've got to do something you've never done.
#
import unittest
import math

def euclidean_norm(data):
    """Euclidean norm"""
    x = data[0]
    y = data[1]

    # The math.hypot() method returns the Euclidean norm. The Euclidian norm is the distance from the origin to the coordinates given.
    # this method was used only to find the hypotenuse of a right-angled triangle: sqrt(x*x + y*y).
    # or n-dimensional cases, the coordinates passed are assumed to be like (x1, x2, x3, ..., xn). So Euclidean length from the origin is calculated by sqrt(x1*x1 + x2*x2 +x3*x3 .... xn*xn).
    #return math.sqrt(x*x + y*y)
    return math.hypot(x, y)

# Use the similarity to association RADAR's detection and Camera's detection.
def euclidean_distance(v0, v1):
    """
    在数据完整(无维度数据缺失)的情况下, 维度间的衡量单位是一致的, 否则需要标准化处理
    他即包括了尺度，也包括了方向.
    dist(A, B) = math.hypot(A - B)
    越小，二者越相似
    """
    x_diff = v0[0] - v1[0]
    y_diff = v0[1] - v1[1]

    return euclidean_norm((x_diff, y_diff))

def cosine_similarity(d1, d2):
    x1 = d1[0]
    y1 = d1[1]

    x2 = d2[0]
    y2 = d2[1]

    return (x1*x2 + y1*y2)/(euclidean_norm(d1) * euclidean_norm(d2))

def cosine_distance(v0, v1):
    """
    取值范围 [0, 2], 值越小，越相似
    """
    return 1 - cosine_similarity(v0, v1)

def junk_distance(v0, v1):
    # bad distance, which leads into the bug
    return math.fabs(euclidean_norm(v0) - euclidean_norm(v1))


# Usage: use as the score: std::exp(1 / similarity)
#
class _Test(unittest.TestCase):
    def setUp(self):
        # The mission is to associate the RADARs to Camera's detection.
        # Here are the detection's velocities and plotted in the vehicle frame.
        #        +y
        #        .
        #        .
        #  *     .
        #  r1    .
        #        .              c
        #.......................*... +x
        #        .      *r2
        #        .   *r3
        #        .
        #        .
        #
        self.radar1 = (-0.893025, 0.176149)
        self.radar2 = (0.857934, -0.0411348)
        self.radar3 = (0.857934, -0.0511348)
        self.camera_detection = (1.16218, 0)

    def testEuclideanDistance(self):
        c_r1 = euclidean_distance(self.radar1, self.camera_detection)
        c_r2 = euclidean_distance(self.radar2, self.camera_detection)
        c_r3 = euclidean_distance(self.radar3, self.camera_detection)
        self.assertGreater(c_r1, c_r2)
        self.assertGreater(c_r1, c_r3)
        self.assertGreater(c_r3, c_r2)

    def testCosineDistance(self):
        c_r1 = cosine_distance(self.radar1, self.camera_detection)
        c_r2 = cosine_distance(self.radar2, self.camera_detection)
        c_r3 = cosine_distance(self.radar3, self.camera_detection)
        self.assertGreater(c_r1, c_r2)
        self.assertGreater(c_r1, c_r3)
        self.assertGreater(c_r3, c_r2)

    def testJunkDistance(self):
        c_r1 = junk_distance(self.radar1, self.camera_detection)
        c_r2 = junk_distance(self.radar2, self.camera_detection)
        c_r3 = junk_distance(self.radar3, self.camera_detection)
        print("c_r1:{}, c_r2: {}, c_r3: {}".format(c_r1, c_r2, c_r3))


    def tearDown(self):
        """
        Bye
        """
        pass

if __name__ == '__main__':
    unittest.main()


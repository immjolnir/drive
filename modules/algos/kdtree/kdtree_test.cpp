#include <gtest/gtest.h>

#include <array>
#include <chrono>
#include <random>

#include "kdtree/kdtree.h"

// user-defined point type
// inherits std::array in order to use operator[]
class MyPoint : public std::array<double, 2> {
  public:
    // dimension of space (or "k" of k-d tree)
    // KDTree class accesses this member
    static const int DIM = 2;

    // the constructors
    MyPoint() {}

    MyPoint(double x, double y) {
        (*this)[0] = x;
        (*this)[1] = y;
    }
};

#define MAX_X 100
#define MAX_Y 256
#define N 128

TEST(KDTree, build) {
    std::vector<MyPoint> points;
    points.reserve(N);

    // https://stackoverflow.com/questions/19665818/generate-random-numbers-using-c11-random-library
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 mt_rand(seed);

    for (int i = 0; i < N; ++i) {
        long long x_rand = mt_rand();  // type: long
        int x = x_rand % MAX_X;

        long long y_rand = mt_rand();  // type: long
        int y = y_rand % MAX_X;
        // std::cout << x << ", " << y << std::endl;

        points.emplace_back(x, y);
    }

    EXPECT_EQ(N, points.size());

    KDTree<MyPoint> tree(points);
    tree.build();
}
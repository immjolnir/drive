#include <gtest/gtest.h>

#include <string>
#include <vector>

template <typename T>
struct Node {
    Node(T v) : value(v), left(nullptr), right(nullptr) {}

    using Ptr = typename std::shared_ptr<Node<T>>;

    T value;
    std::shared_ptr<Node> left, right;
};

template <typename T>
class KDTree {
  public:
    void build(const std::vector<T>& vec) {
        if (vec.empty()) return;

        auto iter = vec.begin();

        _root = std::make_shared<Node<T>>(*iter);

        while (iter != vec.end()) {
            auto val = iter->value;
            size_t selected_dim = current_dim % T::DIM;
            // 1. find the right place to add the this new element
            // todo:

            // 2. Balance it or not?
            // todo:
            ++current_dim;
        }
    }

  private:
    typename Node<T>::Ptr _root = nullptr;
    size_t current_dim = 0;
};

// multiple dimensions data
template <typename T, size_t number_of_elements>
struct MDD {
    std::array<T, number_of_elements> data;
    static constexpr size_t DIM = number_of_elements;

    // Special operators:
    // Element access.
    T& operator[](size_t idx) { return data[idx]; }

    const T& operator[](size_t idx) const {
        assert(idx < DIM);
        return data[idx];
    }
};

/*
Array's implementation:

template <typename _Tp, std::size_t _Nm>
_GLIBCXX20_CONSTEXPR inline bool operator<(const array<_Tp, _Nm>& __a, const array<_Tp, _Nm>& __b) {
    return std::lexicographical_compare(__a.begin(), __a.end(), __b.begin(), __b.end());
}
*/

TEST(kd_tree, basic_example) {
    using PlaneCoordinates = MDD<int, 2>;

    PlaneCoordinates p0;
    p0.data[0] = 1;
    p0.data[1] = 5;

    EXPECT_EQ(2, PlaneCoordinates::DIM);
    EXPECT_EQ(1, p0[0]);
    EXPECT_EQ(5, p0[1]);

    using Words = MDD<std::string, 3>;
    Words w;
    w[0] = "Hello";
    w[1] = "World";
    w[2] = "!";
    EXPECT_EQ(3, Words::DIM);
    EXPECT_EQ("Hello", w[0]);
    EXPECT_EQ("World", w[1]);
    EXPECT_EQ("!", w[2]);
}

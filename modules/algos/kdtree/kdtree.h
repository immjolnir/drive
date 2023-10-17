#pragma once

#include <memory>

struct Node {
    using Ptr = std::shared_ptr<Node>;

    int value;     // index to the real value
    int axis = 0;  //!< dimension's axis
    Ptr left, right;

    Node() : value(-1), axis(0), left(nullptr), right(nullptr) {}
};

template <typename T>
class KDTree {
  public:
    static constexpr int DIM = T::DIM;

    KDTree();

    KDTree(const std::vector<T>& data);

    void build();

    void build(const std::vector<T>& data);

    Node::Ptr build(int* indices, int npoints, int depth);

  private:
    Node::Ptr _root;
    std::vector<T> _data;  // multiple dimension data
};

template <typename T>
KDTree<T>::KDTree() : _root(nullptr) {}

// error: mem-initializer for 'KDTree<T>::_data' follows constructor delegation
template <typename T>
KDTree<T>::KDTree(const std::vector<T>& data) : _root(nullptr), _data(data) {}

template <typename T>
void KDTree<T>::build() {
    if (_data.empty()) return;

    std::vector<int> indices(_data.size());
    std::iota(std::begin(indices), std::end(indices), 0);
    _root = build(indices.data(), (int)_data.size(), 0);
}

template <typename T>
void KDTree<T>::build(const std::vector<T>& data) {
    _data = data;
    build();
}

template <typename T>
Node::Ptr KDTree<T>::build(int* indices, int npoints, int depth) {
    // Recursively building the KD Tree
    if (npoints <= 0) return nullptr;  // exit condition

    const int axis = depth % DIM;
    const int mid = (npoints - 1) / 2;

    std::nth_element(indices, indices + mid, indices + npoints,
                     [&](int lhs, int rhs) { return _data[lhs][axis] < _data[rhs][axis]; });

    Node::Ptr node = std::make_shared<Node>();
    node->value = indices[mid];
    node->axis = axis;

    node->left = build(indices, mid - 1, depth + 1);
    node->right = build(indices + mid + 1, npoints - mid - 1, depth + 1);
    return node;
}
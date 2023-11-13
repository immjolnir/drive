#include <array>
#include <numeric>  // std::iota

namespace algo {

template <unsigned int N>
struct UnionFindSet {
    std::array<int, N> parents;

    void init() { std::iota(parents.begin(), parents.end(), 0); }

    int find(int nodeid) {
        int root = parents[nodeid];
        // TODO: Path Compression
        return root;
    }

    void union_them(int node1, int node2) {
        auto root1 = find(node1);
        auto root2 = find(node2);
        if (root1 == root2) {
            return;
        } else {
            // TODO: who's from and who's to?
            auto from = root1;
            auto to = root2;
            parents[from] = to;
        }
    }
};
}  // namespace algo
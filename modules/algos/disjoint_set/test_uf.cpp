#include "uf.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <utility>
#include <vector>

template <typename Iter>
void visualize(Iter begin, Iter end) {
    // cv::Mat img = cv::Mat::zeros(cv::Size(WIDTH, HEIGHT), CV_8UC3);
    // for (; begin != end; ++begin) {
    //     cv::circle(img, cv::Point2d(x, y), 1, cv::Scalar(0, 255, 255), -1);
    // }
    int root_id = 0;
    std::map<int, std::vector<int>> trees;
    auto iter = begin;
    for (; iter != end; ++iter) {
        int node_id = *iter;
        auto found = trees.find(root_id);
        if (found == trees.end()) {
            trees[root_id].emplace_back(node_id);
        } else {
            found->second.emplace_back(node_id);
        }
        ++root_id;
    }

    // Merge the trees
    // https://stackoverflow.com/questions/110157/how-to-retrieve-all-keys-or-values-from-a-stdmap-and-put-them-into-
    // a-vector
    // {
    //     auto root_ids = std::views::keys(trees);  // since c++20
    //     auto begin = root_ids.begin();
    //     for (; begin != root_ids.end(); ++begin) {
    //         auto root_id = *begin;
    //         auto node_ids = trees[root_id];
    //         // if(binary_search(vector.begin(), vector.end(), item)){
    //         auto target_tree = std::find(node_ids.begin(), node_ids.end(), root_id);
    //         if (target_tree != node_ids.end()) {
    //             // merge them
    //             // C++ append vector into another vector [duplicate]
    //             // v1.insert(v1.end(), v2.begin(), v2.end());
    //             target_tree->insert(target_tree->end(), node_ids.begin(), node_ids.end());
    //             trees.erase(root_id);
    //         }
    //     }
    // }
    for (auto iter = trees.begin(); iter != trees.end(); ++iter) {
        std::cout << iter->first;
        if (!iter->second.empty()) {
            std::cout << "::";
            for (auto it : iter->second) {
                std::cout << it;
            }
            std::cout << std::endl;
        }
    }
}

/**

1 ---- 5        3               6
|      |         \
|      |          \
|      |          2 ---- 0
4 ---- 8          |
                  |
                  7
*/
#define N 9
// Bad selected data struct: std::map<int, int> EDGES = ...
static const std::vector<std::pair<int, int>> EDGES = {{0, 2}, {1, 4}, {1, 5}, {2, 3}, {2, 7}, {4, 8}, {5, 8}};

TEST(DisjointSet, correct_edges) { EXPECT_EQ(7, EDGES.size()); }

TEST(DisjointSet, quick_find) {
    algo::QuickFind<N> uf;

    for (auto& edge : EDGES) {
        auto from = edge.first;
        auto to = edge.second;
        uf.Union(from, to);
    }

    // std::cout << "Number of the sets: " << uf.Count() << std::endl;
    // std::cout << "Table: \n" << uf << std::endl;
    EXPECT_EQ(3, uf.Count());
    EXPECT_THAT(uf.parent, testing::ElementsAre(7, 8, 7, 7, 8, 8, 6, 7, 8));
}

TEST(DisjointSet, quick_union) {
    algo::QuickUnion<N> uf;

    for (auto& edge : EDGES) {
        auto from = edge.first;
        auto to = edge.second;
        uf.Union(from, to);
    }

    // std::cout << "Number of the sets: " << uf.Count() << std::endl;
    // std::cout << "Table: \n" << uf << std::endl;
    EXPECT_EQ(3, uf.Count());
    EXPECT_THAT(uf.parent, testing::ElementsAre(2, 4, 3, 7, 5, 8, 6, 7, 8));
}

TEST(DisjointSet, weighted_quick_union) {
    algo::WeightedQuickUnion<N> uf;

    for (auto& edge : EDGES) {
        auto from = edge.first;
        auto to = edge.second;
        uf.Union(from, to);
    }

    // std::cout << "Number of the sets: " << uf.Count() << std::endl;
    // std::cout << "Table: \n" << uf << std::endl;
    EXPECT_EQ(3, uf.Count());
    EXPECT_THAT(uf.parent, testing::ElementsAre(0, 1, 0, 0, 1, 1, 6, 0, 1));
}

TEST(DisjointSet, union_find) {
    algo::UnionFind<N> uf;

    for (auto& edge : EDGES) {
        auto from = edge.first;
        auto to = edge.second;
        uf.Union(from, to);
    }

    // std::cout << "Number of the sets: " << uf.Count() << std::endl;
    // std::cout << "Table: \n" << uf << std::endl;
    EXPECT_EQ(3, uf.Count());
    EXPECT_THAT(uf.parent, testing::ElementsAre(0, 1, 0, 0, 1, 1, 6, 0, 1));
}
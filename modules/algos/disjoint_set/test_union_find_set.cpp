#include "union_find_set.h"

#include <gtest/gtest.h>

#include <map>
#include <ranges>

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

TEST(DisjointSet, t1) {
    algo::UnionFindSet<9> uf;
    uf.init();

    std::map<int, int> edges = {{0, 2}, {1, 4}, {1, 5}, {2, 3}, {2, 7}, {4, 8}, {5, 8}};
    // https://stackoverflow.com/questions/110157/how-to-retrieve-all-keys-or-values-from-a-stdmap-and-put-them-into-
    // a-vector
    auto ks = std::views::keys(edges);  // since c++20
    auto begin = ks.begin();
    for (; begin != ks.end(); ++begin) {
        auto from = *begin;
        auto to = edges[from];
        uf.union_them(from, to);
    }

    // visualize(uf.parents.begin(), uf.parents.end());
    for (int i = 0; i < uf.parents.size(); ++i) {
        std::cout << i << "=>" << uf.parents[i] << std::endl;
    }
}
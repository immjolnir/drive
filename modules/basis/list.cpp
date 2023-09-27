#include <gtest/gtest.h>
#include <algorithm>  // std::find
#include <list>

/**
 * https://en.cppreference.com/w/cpp/container/list
 *
 * std::list is a container that supports constant time insertion and removal of elements from anywhere in the
 * container. Fast random access is not supported. It is usually implemented as a doubly-linked list. Compared to
 * std::forward_list this container provides bidirectional iteration capability while being less space efficient.
 *
 * Adding, removing and moving the elements within the list or across several lists does not invalidate the iterators or
 * references. An iterator is invalidated only when the corresponding element is deleted.
 *
 *
 * begin/end definition: https://en.cppreference.com/w/cpp/container/list/end
 * begin: Returns an iterator to the first element of the list.
 *   If the list is empty, the returned iterator will be equal to end().
 * end: Returns an iterator to the element following the last element of the list.
 *
       front/head               back/tail
           V                        V
        .-----+---+---+---+---+---+---+---
        |begin|   |   |   |   |   |   | end(past-the-last-element)
        '-----+---+---+---+---+---+---+---
          ^                         ^
    push_front                  push_back
    emplace_front               emplace_back
    pop_front                   pop_back


 * When adding a new element with push_back()/emplace_back() and deleting the old element with pop_front(),
 * the front() is the oldest.
 *
 * By default, the front() is the oldest and the back is the newest.
 *
 * emplace_back: constructs an element in-place at the end
 * push_back: adds an element to the end
 *
 * front: access the frist element
 * back: access the last element
 */

TEST(list, treat_it_as_circlar_buffer) {
    std::list<int> my_list = {1, 2, 3, 4};
    EXPECT_EQ(1, my_list.front());  // the first(oldest) element is the head value
    EXPECT_EQ(4, my_list.back());   // the last(newest) element is the tail value
    EXPECT_EQ(my_list.front(), *my_list.begin());
    EXPECT_EQ(my_list.back(), *(std::prev(my_list.end())));

    my_list.emplace_front(6);
    my_list.push_back(5);
    EXPECT_EQ(6, my_list.front());
    EXPECT_EQ(5, my_list.back());

    // Print list.
    std::for_each(my_list.begin(), my_list.end(), [](const int n) { std::cout << n << ' '; });
    std::cout << '\n';
}

TEST(list, pop_front_pop_back) {
    std::list<int> my_list = {1, 2, 3, 4};
    EXPECT_EQ(1, my_list.front());  // the first(oldest) element is the head value
    EXPECT_EQ(4, my_list.back());   // the last(newest) element is the tail value

    my_list.pop_front();
    EXPECT_EQ(2, my_list.front());

    my_list.pop_back();
    EXPECT_EQ(3, my_list.back());
}

TEST(list, removeOldMessages) {
    std::list<int> my_list = {1, 2, 3, 4};

    // Removed msgs [0, n] (n inclusive)
    auto removeOldMessages = [&my_list](uint8_t n) {
        for (int i = 0; i <= n; ++i) {
            my_list.pop_front();  // remove the oldest element from head
        }
    };

    EXPECT_EQ(4, my_list.size());
    removeOldMessages(0);
    EXPECT_EQ(3, my_list.size());
}

TEST(list, deleting) {
    std::list<int> my_list = {1, 2, 3, 4};

    // Removed msgs [0, n] (n inclusive)
    auto removeOldMessages = [&my_list](uint8_t n) {
        for (int i = 0; i <= n; ++i) {
            my_list.pop_front();  // remove the oldest element from head
        }
    };

    int16_t closest_idx = -1;  // find the 3
    int16_t current_idx = -1;
    std::stringstream ss;
    for (const auto& data : my_list) {
        ss << data;
        ++current_idx;
        if (data == 3) {
            closest_idx = current_idx;
        }
    }
    // Remove all data upto the closest_idx, we want out relevant data to be at front of the list
    if (closest_idx > 0) {
        removeOldMessages(closest_idx - 1);
    }

    // We couldn't find a match, but we have old data that can be deleted
    if (closest_idx == -1 && current_idx >= 0) {
        removeOldMessages(current_idx);
    }

    EXPECT_EQ("1234", ss.str());
    EXPECT_EQ(2, closest_idx);
    EXPECT_EQ(3, current_idx);
    EXPECT_EQ(2, my_list.size());
    EXPECT_EQ(3, my_list.front());
    EXPECT_EQ(4, my_list.back());

    for (auto& i : my_list) {
        std::cout << i << std::endl;
    }
}

// Is there a better way than above to find the exact element and remove the oldest?
TEST(list, deleting_batch) {
    // erase
    // Erases the specified elements from the container.
    // * Removes the element in pos
    // iterator erase( iterator pos );
    // iterator erase( const_iterator pos );
    //
    // * Removes the elements in the range [first, last).
    // iterator erase( iterator first, iterator last );
    // iterator erase( const_iterator first, const_iterator last );
    std::list<int> my_list = {1, 2, 3, 4};
    /*
            .---+---+---+---+---+---+---
            | 1 | 2 | 3 | 4 |   |   |
            '---+---+---+---+---+---+---
                      ^
                    found
            |<-  ->| deleting ranges
    */
    auto found = std::find(my_list.begin(), my_list.end(), 3);
    if (found != my_list.end()) {
        my_list.erase(my_list.begin(), found);
    }
    EXPECT_EQ(2, my_list.size());
    EXPECT_EQ(3, my_list.front());
    EXPECT_EQ(4, my_list.back());
}

// Actually, if there's no random RW, we can use vector/array.
// The underlying of the list is double-linked list.
//
//  > std::list is a container that supports constant time insertion and removal of elements from anywhere in the
//  > container. Fast random access is not supported. It is usually implemented as a doubly-linked list.

// Radom inserting
TEST(list, radom_insert) {
    std::list<int> my_list = {1, 2, 3, 4};
    EXPECT_EQ(4, my_list.size());
    EXPECT_EQ(1, my_list.front());
    EXPECT_EQ(4, my_list.back());

    // Insert an integer before 3 by searching
    auto it = std::find(my_list.begin(), my_list.end(), 3);
    EXPECT_TRUE(it != my_list.end());
    // Inserts elements at the specified location in the container.
    my_list.insert(it, 42);
    EXPECT_EQ(5, my_list.size());
    EXPECT_EQ(1, my_list.front());
    EXPECT_EQ(4, my_list.back());

    {
        // Remove the element: 42
        auto found = std::find(my_list.begin(), my_list.end(), 42);
        EXPECT_TRUE(found != my_list.end());
        my_list.erase(found);          // random deleting
        EXPECT_EQ(4, my_list.size());  // back to original size
        EXPECT_EQ(1, my_list.front());
        EXPECT_EQ(4, my_list.back());
    }
}

TEST(list, clear) {
    std::list<int> my_list = {1, 2, 3, 4};
    EXPECT_EQ(4, my_list.size());
    my_list.clear();
    EXPECT_EQ(0, my_list.size());
    EXPECT_TRUE(my_list.empty());
}

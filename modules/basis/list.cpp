#include <gtest/gtest.h>
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
  back(tail)       front(head)
    |                   |
    v                   v
    +---+---+---+---+---+
    |   |   |   |   |   |
    +---+---+---+---+---+
push_back             push_front
emplace_back          emplace_front
pop_back              pop_front

 * When adding a new element with push_back()/emplace_back() and deleting the old element with pop_front(),
 * the front() is the oldest.
 *
 * By default, the front() is the oldest and the back is the newest.
 *
 * emplace_back: constructs an element in-place at the end
 * push_back: adds an element to the end
 * pop_front: removes the first element
 *
 * front: access the frist element
 * back: access the last element
 */

TEST(list, treat_it_as_circlar_buffer) {
    std::list<int> my_list = {1, 2, 3, 4};
    EXPECT_EQ(1, my_list.front());  // the first(oldest) element is the head value
    EXPECT_EQ(4, my_list.back());   // the last(newest) element is the tail value

    my_list.emplace_front(6);
    my_list.push_back(5);
    EXPECT_EQ(6, my_list.front());
    EXPECT_EQ(5, my_list.back());
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

// Is there a better way to find the exact element and remove the oldest?

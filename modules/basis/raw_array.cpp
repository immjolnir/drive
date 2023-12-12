#include <algorithm>  // sort
#include <cstring>    // memcpy
#include <numeric>    // std::accumulate
#include <string>
#include <vector>

#include <gtest/gtest.h>
using namespace std;

// Please use std::array in c++.
// TEST(raw_array, sort_empty_array) {
//     // error: zero-size array 'arr'
//     int arr[] = {};
//     int n = sizeof(arr) / sizeof(arr[0]);
//     std::sort(arr, arr + n);
//     EXPECT_EQ(1, n);
// }

TEST(raw_array, sort_array) {
    int arr[] = {1, 5, 7, 2, 4};
    int n = sizeof(arr) / sizeof(arr[0]);
    std::sort(arr, arr + n);
    // construct the vector after you've gotten the array and array size
    // https://stackoverflow.com/questions/259297/how-do-you-copy-the-contents-of-an-array-to-a-stdvector-in-c-without-looping
    std::vector<int> actual(arr, arr + n);
    std::vector<int> expected({1, 2, 4, 5, 7});
    EXPECT_EQ(expected, actual);
}

// Using std::copy, this still iterates in the background, but you don't have to type out the code.
std::vector<int> foo(int* data, int size) {
    std::vector<int> my_data;  // normally a class variable
    std::copy(data, data + size, std::back_inserter(my_data));
#pragma GCC diagnostic ignored "-Wpessimizing-move"
    // error: moving a local object in a return statement prevents copy elision [-Werror=pessimizing-move]
    return std::move(my_data);
}

TEST(raw_array, covert_to_vector_with_std_copy) {
    int arr[] = {1, 5, 7, 2, 4};
    int n = sizeof(arr) / sizeof(arr[0]);
    auto actual = foo(arr, n);
    std::vector<int> expected({1, 5, 7, 2, 4});
    EXPECT_EQ(expected, actual);
}

// Using regular memcpy. This is probably best used for basic data types (i.e. int) but not for more complex arrays of
// structs or classes.
TEST(raw_array, covert_to_vector_with_std_memcpy) {
    int arr[] = {1, 5, 7, 2, 4};
    size_t size = sizeof(arr) / sizeof(arr[0]);

    // error: conversion to 'std::vector<int>::size_type' {aka 'long unsigned int'} from 'int' may change the sign of
    // the result
    std::vector<int> actual(size);
    // void* memcpy( void* dest, const void* src, std::size_t count );
    std::memcpy(&actual[0], arr, sizeof(arr));

    std::vector<int> expected({1, 5, 7, 2, 4});
    EXPECT_EQ(expected, actual);
}

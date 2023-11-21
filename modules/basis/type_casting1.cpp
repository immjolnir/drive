#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>

/**
 *
 * User Defined operator provides type casting function.
 *
 */

// Similar to https://github.com/strawlab/ros_comm/blob/master/utilities/rostime/include/ros/time.h
class Time {
  public:
    uint32_t sec, nsec;

    Time() : sec(0), nsec(0) { std::cout << "ctor0" << std::endl; }

    Time(uint32_t _sec, uint32_t _nsec) : sec(_sec), nsec(_nsec) { std::cout << "ctor1" << std::endl; }

    Time(const Time& other) {
        std::cout << "copy ctor" << std::endl;
        // Called this
        sec = other.sec;
        nsec = other.nsec;
    }

    Time(Time&&) { std::cout << "move ctor" << std::endl; }

    Time& operator=(const Time&) {
        std::cout << "copy assignment" << std::endl;
        return *this;
    }

    Time& operator=(Time&& instance) {
        std::cout << "move assignment" << std::endl;
        // std::swap(*this, instance);
        // std::move(
        return *this;
    }
};

struct Tester {
    Tester(const Time& time) : t{time} {}

    // Tester(Time&& time): t(std::move(time)) {}
    // Cast the current object to Time
    operator Time() const {
        std::cout << "implicit convertion" << std::endl;
        return t;
    }

    operator const Time&() const {  // 这个方法签名更详细，只能调用到了；如果想调用上面的方法，只能把这个方法注释掉
        std::cout << "implicit convertion with ret: const Time&" << std::endl;
        return t;
    }

    Time t{};
};

TEST(type_casting, operator_casting) {
    Time gtime(1, 2);
    Tester tester(gtime);
    /*
    error: conversion from 'Tester' to 'const Time' is ambiguous

    43:5: note: candidate: 'Tester::operator Time() const'
    43 |     operator Time() const {
      |     ^~~~~~~~
    48:5: note: candidate: 'Tester::operator const Time&() const'
    48 |     operator const Time&() const {
      |     ^~~~~~~~

    const Time t = tester;
    */
    const Time& t = tester;
    EXPECT_EQ(1, t.sec);
    EXPECT_EQ(2, t.nsec);
}
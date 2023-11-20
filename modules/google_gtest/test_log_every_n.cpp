#include <gtest/gtest.h>

#include <iostream>

#define MAX_LOOP 1e2
#define N 10

void log_every_n_wrong_behavior(int msg_id) {
    static int counter = N;
    // std::cout << "counter=" << counter << ", msg_id=" << msg_id<< std::endl;
    if (counter == 1) {
        std::cout << "*** counter=" << counter << ", msg_id=" << msg_id << std::endl;
        counter = N;
    } else {
        counter--;
    }
}

/*
*** counter=1, msg_id=9
*** counter=1, msg_id=19
*** counter=1, msg_id=29
*** counter=1, msg_id=39
*** counter=1, msg_id=49
*** counter=1, msg_id=59
*** counter=1, msg_id=69
*** counter=1, msg_id=79
*** counter=1, msg_id=89
*** counter=1, msg_id=99
*/

// LOG_EVERY_N(severity, n)
// src/glog/logging.h.in
void log_every_n(int n, int msg_id) {
    static int LOG_OCCURRENCES = 0, LOG_OCCURRENCES_MOD_N = 0;
    ++LOG_OCCURRENCES;

    if (++LOG_OCCURRENCES_MOD_N > n) LOG_OCCURRENCES_MOD_N -= n;
    if (LOG_OCCURRENCES_MOD_N == 1)
        std::cout << "*** counter=" << LOG_OCCURRENCES_MOD_N << ", occurrences=" << LOG_OCCURRENCES
                  << ", msg_id=" << msg_id << std::endl;
}

/**
*** counter=1, occurrences=1, msg_id=0
*** counter=1, occurrences=11, msg_id=10
*** counter=1, occurrences=21, msg_id=20
*** counter=1, occurrences=31, msg_id=30
*** counter=1, occurrences=41, msg_id=40
*** counter=1, occurrences=51, msg_id=50
*** counter=1, occurrences=61, msg_id=60
*** counter=1, occurrences=71, msg_id=70
*** counter=1, occurrences=81, msg_id=80
*** counter=1, occurrences=91, msg_id=90
*** counter=1, occurrences=101, msg_id=100
*/

TEST(gtest, log_every_n_wrong_behavior) {
    int msg_id = 0;
    while (msg_id <= MAX_LOOP) {
        log_every_n_wrong_behavior(msg_id);
        msg_id++;
    }
}

TEST(gtest, log_every_n) {
    int msg_id = 0;
    while (msg_id <= MAX_LOOP) {
        log_every_n(N, msg_id);
        msg_id++;
    }
}

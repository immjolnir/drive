#include <gtest/gtest.h>
#include <iostream>

#include "fred.h"

TEST(ReferenceCountingPatternTest, using_shared_ptr_shadow_copy) {
    Fred global = Fred::create1("Mario", 1);

    EXPECT_EQ(1, global.use_count());
    global.sampleInspectorMethod();
    global.sampleMutatorMethod();

    Fred other = global;
    EXPECT_EQ(2, global.use_count());
    EXPECT_EQ(2, other.use_count());
    other.sampleInspectorMethod();
    other.sampleMutatorMethod();
}

TEST(ReferenceCountingPatternTest, using_shared_ptr_deep_copy) {
    Fred global = Fred::create1("Mario", 1);

    EXPECT_EQ(1, global.use_count());
    global.sampleInspectorMethod();
    global.sampleMutatorMethod();

    Fred other = global.clone();
    EXPECT_EQ(1, global.use_count());
    EXPECT_EQ(1, other.use_count());
    other.sampleInspectorMethod();
    other.sampleMutatorMethod();
}

TEST(ReferenceCountingPatternTest, using_shared_ptr_shadow_copy2) {
    Fred global = Fred::create2(3.14f, 1.735f);

    EXPECT_EQ(1, global.use_count());
    global.sampleInspectorMethod();
    global.sampleMutatorMethod();

    Fred other = global;
    EXPECT_EQ(2, global.use_count());
    EXPECT_EQ(2, other.use_count());
    other.sampleInspectorMethod();
    other.sampleMutatorMethod();
}

TEST(ReferenceCountingPatternTest, using_shared_ptr_deep_copy2) {
    Fred global = Fred::create2(3.14f, 1.735f);

    EXPECT_EQ(1, global.use_count());
    global.sampleInspectorMethod();
    global.sampleMutatorMethod();

    Fred other = global.clone();
    EXPECT_EQ(1, global.use_count());
    EXPECT_EQ(1, other.use_count());
    other.sampleInspectorMethod();
    other.sampleMutatorMethod();
}

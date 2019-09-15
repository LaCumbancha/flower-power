//
// Created by darius on 14/9/19.
//

#include "gtest/gtest.h"

class testExamples : public ::testing::Test {

};

TEST(testExamples, TestEqualFails) {
    EXPECT_EQ(1,2);
}

TEST(testExamples, TestEqualsSucceeds) {
    EXPECT_EQ(2,2);
}


#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "timer.h"

// Test fixture for Timer tests.

class TimerTest : public ::testing::Test {
protected:
    void SetUp() override { stop(); }
    void TearDown() override { stop(); }
};

TEST_F(TimerTest, StartReturnsZero) {
    EXPECT_EQ(start(), 0);
}

TEST_F(TimerTest, StopWithoutStartReturnsError) {
    EXPECT_EQ(stop(), -1);
}

TEST_F(TimerTest, StopBeforeOneSecondReturnsZero) {
    start();
    EXPECT_EQ(stop(), 0);
}

TEST_F(TimerTest, StopReturnsFullSeconds) {
    start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    EXPECT_EQ(stop(), 1);
}

TEST_F(TimerTest, SecondStopReturnsError) {
    start();
    stop();
    EXPECT_EQ(stop(), -1);
}

TEST_F(TimerTest, RepeatedStartKeepsOriginalStartTime) {
    start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    EXPECT_EQ(start(), 0);
    EXPECT_EQ(stop(), 1);
}
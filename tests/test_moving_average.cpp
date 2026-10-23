#include <gtest/gtest.h>

#include <stdexcept>

#include "cicd/moving_average.hpp"

using cicd::MovingAverage;

TEST(MovingAverageTest, ThrowsOnZeroWindow) {
    EXPECT_THROW(MovingAverage ma(0U), std::invalid_argument);
}

TEST(MovingAverageTest, ThrowsWhenNoSampleAvailable) {
    MovingAverage ma(3U);
    EXPECT_THROW(ma.value(), std::logic_error);
}

TEST(MovingAverageTest, SingleSampleReturnsItself) {
    MovingAverage ma(3U);
    ma.add(42.0);
    EXPECT_DOUBLE_EQ(ma.value(), 42.0);
    EXPECT_EQ(ma.count(), 1U);
}

TEST(MovingAverageTest, PartialWindowAveragesAvailableSamples) {
    MovingAverage ma(4U);
    ma.add(2.0);
    ma.add(4.0);
    EXPECT_DOUBLE_EQ(ma.value(), 3.0);
    EXPECT_EQ(ma.count(), 2U);
    EXPECT_EQ(ma.window_size(), 4U);
    EXPECT_FALSE(ma.full());
}

TEST(MovingAverageTest, FullWindowDropsOldestSample) {
    MovingAverage ma(3U);
    ma.add(1.0);
    ma.add(2.0);
    ma.add(3.0);
    EXPECT_TRUE(ma.full());
    EXPECT_DOUBLE_EQ(ma.value(), 2.0);
    ma.add(4.0);
    EXPECT_DOUBLE_EQ(ma.value(), 3.0);
    ma.add(5.0);
    EXPECT_DOUBLE_EQ(ma.value(), 4.0);
    EXPECT_EQ(ma.count(), 3U);
}

TEST(MovingAverageTest, ResetClearsState) {
    MovingAverage ma(3U);
    ma.add(1.0);
    ma.add(2.0);
    ma.reset();
    EXPECT_EQ(ma.count(), 0U);
    EXPECT_FALSE(ma.full());
    EXPECT_THROW(ma.value(), std::logic_error);
    ma.add(10.0);
    EXPECT_DOUBLE_EQ(ma.value(), 10.0);
}

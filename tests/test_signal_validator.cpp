#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>

#include "cicd/signal_validator.hpp"

using cicd::SignalLimits;
using cicd::SignalStatus;
using cicd::SignalValidator;

namespace {
const SignalLimits kLimits{0.0, 250.0, 15.0};
}

TEST(SignalValidatorTest, RejectsInvertedRange) {
    EXPECT_THROW(SignalValidator v(SignalLimits{10.0, 0.0, 5.0}), std::invalid_argument);
}

TEST(SignalValidatorTest, RejectsNegativeDelta) {
    EXPECT_THROW(SignalValidator v(SignalLimits{0.0, 10.0, -1.0}), std::invalid_argument);
}

TEST(SignalValidatorTest, FirstSampleInRangeIsValid) {
    SignalValidator v(kLimits);
    EXPECT_EQ(v.check(100.0), SignalStatus::Valid);
}

TEST(SignalValidatorTest, SampleBelowMinimumIsOutOfRange) {
    SignalValidator v(kLimits);
    EXPECT_EQ(v.check(-0.1), SignalStatus::OutOfRange);
}

TEST(SignalValidatorTest, SampleAboveMaximumIsOutOfRange) {
    SignalValidator v(kLimits);
    EXPECT_EQ(v.check(250.1), SignalStatus::OutOfRange);
}

TEST(SignalValidatorTest, BoundaryValuesAreValid) {
    SignalValidator low(kLimits);
    SignalValidator high(kLimits);
    EXPECT_EQ(low.check(0.0), SignalStatus::Valid);
    EXPECT_EQ(high.check(250.0), SignalStatus::Valid);
}

TEST(SignalValidatorTest, NaNIsOutOfRange) {
    SignalValidator v(kLimits);
    EXPECT_EQ(v.check(std::numeric_limits<double>::quiet_NaN()), SignalStatus::OutOfRange);
}

TEST(SignalValidatorTest, JumpAboveLimitExceedsRate) {
    SignalValidator v(kLimits);
    EXPECT_EQ(v.check(100.0), SignalStatus::Valid);
    EXPECT_EQ(v.check(120.0), SignalStatus::RateExceeded);
}

TEST(SignalValidatorTest, JumpEqualToLimitIsValid) {
    SignalValidator v(kLimits);
    EXPECT_EQ(v.check(100.0), SignalStatus::Valid);
    EXPECT_EQ(v.check(115.0), SignalStatus::Valid);
}

TEST(SignalValidatorTest, RateExceededSampleDoesNotBecomeReference) {
    SignalValidator v(kLimits);
    EXPECT_EQ(v.check(100.0), SignalStatus::Valid);
    EXPECT_EQ(v.check(130.0), SignalStatus::RateExceeded);
    EXPECT_EQ(v.check(110.0), SignalStatus::Valid);
}

TEST(SignalValidatorTest, OutOfRangeSampleDoesNotBecomeReference) {
    SignalValidator v(kLimits);
    EXPECT_EQ(v.check(100.0), SignalStatus::Valid);
    EXPECT_EQ(v.check(300.0), SignalStatus::OutOfRange);
    EXPECT_EQ(v.check(105.0), SignalStatus::Valid);
}

TEST(SignalValidatorTest, ResetClearsReferenceSample) {
    SignalValidator v(kLimits);
    EXPECT_EQ(v.check(100.0), SignalStatus::Valid);
    v.reset();
    EXPECT_EQ(v.check(200.0), SignalStatus::Valid);
}

TEST(SignalValidatorTest, ToStringCoversAllStatuses) {
    EXPECT_STREQ(cicd::to_string(SignalStatus::Valid), "VALID");
    EXPECT_STREQ(cicd::to_string(SignalStatus::OutOfRange), "OUT_OF_RANGE");
    EXPECT_STREQ(cicd::to_string(SignalStatus::RateExceeded), "RATE_EXCEEDED");
}

TEST(SignalValidatorTest, LimitsReturnsConfiguredValues) {
    SignalValidator v(kLimits);
    EXPECT_DOUBLE_EQ(v.limits().min_value, 0.0);
    EXPECT_DOUBLE_EQ(v.limits().max_value, 250.0);
    EXPECT_DOUBLE_EQ(v.limits().max_delta_per_sample, 15.0);
}

TEST(SignalValidatorTest, ToStringHandlesUnknownValue) {
    EXPECT_STREQ(cicd::to_string(static_cast<SignalStatus>(99)), "UNKNOWN");
}

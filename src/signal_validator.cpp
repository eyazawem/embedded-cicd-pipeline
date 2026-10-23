#include "cicd/signal_validator.hpp"

#include <cmath>
#include <stdexcept>

namespace cicd {

SignalValidator::SignalValidator(const SignalLimits& limits) : limits_(limits) {
    if (limits_.min_value > limits_.max_value) {
        throw std::invalid_argument("min_value must not exceed max_value");
    }
    if (limits_.max_delta_per_sample < 0.0) {
        throw std::invalid_argument("max_delta_per_sample must not be negative");
    }
}

SignalStatus SignalValidator::check(double sample) {
    if (std::isnan(sample) || sample < limits_.min_value || sample > limits_.max_value) {
        return SignalStatus::OutOfRange;
    }
    if (has_previous_ && std::fabs(sample - previous_) > limits_.max_delta_per_sample) {
        return SignalStatus::RateExceeded;
    }
    previous_ = sample;
    has_previous_ = true;
    return SignalStatus::Valid;
}

void SignalValidator::reset() noexcept {
    has_previous_ = false;
    previous_ = 0.0;
}

const SignalLimits& SignalValidator::limits() const noexcept {
    return limits_;
}

const char* to_string(SignalStatus status) noexcept {
    switch (status) {
        case SignalStatus::Valid:
            return "VALID";
        case SignalStatus::OutOfRange:
            return "OUT_OF_RANGE";
        case SignalStatus::RateExceeded:
            return "RATE_EXCEEDED";
    }
    return "UNKNOWN";
}

}  // namespace cicd

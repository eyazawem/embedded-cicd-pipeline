#pragma once

namespace cicd {

enum class SignalStatus {
    Valid,
    OutOfRange,
    RateExceeded
};

struct SignalLimits {
    double min_value;
    double max_value;
    double max_delta_per_sample;
};

/// Plausibility check for a sensor signal: value range and maximum change
/// between two consecutive valid samples. A rejected sample never becomes
/// the reference for the next check.
class SignalValidator {
public:
    /// @throws std::invalid_argument if min_value > max_value or
    ///         max_delta_per_sample is negative.
    explicit SignalValidator(const SignalLimits& limits);

    SignalStatus check(double sample);
    void reset() noexcept;
    const SignalLimits& limits() const noexcept;

private:
    SignalLimits limits_;
    bool has_previous_{false};
    double previous_{0.0};
};

const char* to_string(SignalStatus status) noexcept;

}  // namespace cicd

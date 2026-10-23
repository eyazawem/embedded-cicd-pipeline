#pragma once

#include <cstddef>
#include <vector>

namespace cicd {

/// Moving average over the last N samples (fixed-size circular buffer).
class MovingAverage {
public:
    /// @throws std::invalid_argument if window_size is 0.
    explicit MovingAverage(std::size_t window_size);

    void add(double sample);

    /// Average of the samples currently in the window.
    /// @throws std::logic_error if no sample has been added yet.
    double value() const;

    std::size_t count() const noexcept;
    std::size_t window_size() const noexcept;
    bool full() const noexcept;
    void reset() noexcept;

private:
    std::vector<double> buffer_;
    std::size_t next_{0};
    std::size_t count_{0};
};

}  // namespace cicd

#include "cicd/moving_average.hpp"

#include <cstddef>
#include <numeric>
#include <stdexcept>

namespace cicd {

MovingAverage::MovingAverage(std::size_t window_size) : buffer_(window_size, 0.0) {
    if (window_size == 0U) {
        throw std::invalid_argument("window_size must be greater than 0");
    }
}

void MovingAverage::add(double sample) {
    buffer_[next_] = sample;
    next_ = (next_ + 1U) % buffer_.size();
    if (count_ < buffer_.size()) {
        ++count_;
    }
}

double MovingAverage::value() const {
    if (count_ == 0U) {
        throw std::logic_error("no sample available");
    }
    const double sum = std::accumulate(buffer_.begin(),
                                       buffer_.begin() + static_cast<std::ptrdiff_t>(count_), 0.0);
    return sum / static_cast<double>(count_);
}

std::size_t MovingAverage::count() const noexcept { return count_; }

std::size_t MovingAverage::window_size() const noexcept { return buffer_.size(); }

bool MovingAverage::full() const noexcept { return count_ == buffer_.size(); }

void MovingAverage::reset() noexcept {
    next_ = 0U;
    count_ = 0U;
}

} // namespace cicd

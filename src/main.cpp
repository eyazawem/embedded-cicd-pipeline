#include <iostream>

#include "cicd/moving_average.hpp"
#include "cicd/signal_validator.hpp"

int main() {
    const cicd::SignalLimits limits{0.0, 250.0, 15.0};
    cicd::SignalValidator validator(limits);
    cicd::MovingAverage filter(3U);

    const double samples[] = {50.0, 55.0, 60.0, 300.0, 62.0, 95.0, 64.0};

    for (const double sample : samples) {
        const cicd::SignalStatus status = validator.check(sample);
        std::cout << "sample=" << sample << " status=" << cicd::to_string(status);
        if (status == cicd::SignalStatus::Valid) {
            filter.add(sample);
            std::cout << " filtered=" << filter.value();
        }
        std::cout << '\n';
    }
    return 0;
}

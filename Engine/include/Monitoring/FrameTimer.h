#pragma once
#include <deque>
#include "Data/FrameStats.h"

class FrameTimer final {
private:
    double lastTime = -1.0;
    double smoothDelta = 0.016;
    const double deltaTau = 0.1;

    std::deque<double> frameTimes;
    double accumulatedTime = 0.0;
    const double windowDuration = 5.0;

public:
    FrameStats Update(double currentTime);
};
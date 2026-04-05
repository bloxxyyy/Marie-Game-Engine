#include "Monitoring/FrameTimer.h"
#include <algorithm>
#include <cmath>

FrameStats FrameTimer::Update(double currentTime) {
    FrameStats stats{};

    if (lastTime < 0.0) {
        lastTime = currentTime;
        return {0.0, 0.0, 0.0};
    }

    double rawDelta = currentTime - lastTime;
    lastTime = currentTime;
    rawDelta = std::clamp(rawDelta, 0.0001, 0.5);

    double alpha = 1.0 - std::exp(-rawDelta / deltaTau);
    smoothDelta += alpha * (rawDelta - smoothDelta);

    stats.delta = smoothDelta;
    stats.fps = 1.0 / std::max(smoothDelta, 0.0001);

    frameTimes.push_back(rawDelta);
    accumulatedTime += rawDelta;
    while (accumulatedTime > windowDuration && !frameTimes.empty()) {
        accumulatedTime -= frameTimes.front();
        frameTimes.pop_front();
    }

    double avgDelta = 0.0;
    if (!frameTimes.empty()) {
        avgDelta = accumulatedTime / frameTimes.size();
        stats.avgFps = 1.0 / std::max(avgDelta, 0.0001);
    } else {
        stats.avgFps = stats.fps;
    }

    return stats;
}
#pragma once
#include <vector>

struct EngineStats {
    double deltaTime = 0.0;
    double fps = 0.0;
    double avgFPS = 0.0;
    double cpuUsage = 0.0;

    std::vector<double> fpsHistory;
    const size_t maxHistory = 1200;
};

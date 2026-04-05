#pragma once

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

class CpuMonitor final {
private:
    FILETIME prevIdleTime{};
    FILETIME prevKernelTime{};
    FILETIME prevUserTime{};

    double smoothCpu = 0.0;
    bool initialized = false;

    double accumulator = 0.0;
    const double updateInterval = 0.2;
    
    const double tau = 0.5;

public:
    CpuMonitor();

    double Update(double deltaTime);
};
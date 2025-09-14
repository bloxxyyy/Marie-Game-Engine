#pragma once

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#ifdef APIENTRY
#undef APIENTRY
#endif

#include <glad/glad.h>
#include "Data/FrameStats.h"

class Win_Monitoring {
private:
    // CPU tracking
    FILETIME prevIdleTime{};
    FILETIME prevKernelTime{};
    FILETIME prevUserTime{};
    double smoothCPU = 0.0;
    const double cpuSmoothing = 0.1;
    const float cpuUpdateInterval = 0.2f;
    float cpuTimeAccumulator = 0.0f;

    // GPU tracking
    GLuint gpuQuery = 0;

    // Frame timing
    float lastFrameTime = 0.0f;
    float smoothDelta = 0.016f;
    const float deltaSmoothing = 0.1f;
    float fpsAccumulator = 0.0f;
    int frameCount = 0;

public:
    Win_Monitoring();
    ~Win_Monitoring();

    double GetCPUUsage(float deltaTime);
    FrameStats UpdateFrameTiming(float currentTime);
};

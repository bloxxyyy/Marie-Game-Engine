#include "Monitoring/Win_Monitoring.h"
#include <GLFW/glfw3.h>

#include <algorithm>

Win_Monitoring::Win_Monitoring() {
    glGenQueries(1, &gpuQuery);
    lastFrameTime = static_cast<float>(glfwGetTime());
    
    FILETIME idleTime, kernelTime, userTime;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);
    prevIdleTime = idleTime;
    prevKernelTime = kernelTime;
    prevUserTime = userTime;
}

Win_Monitoring::~Win_Monitoring() {
    glDeleteQueries(1, &gpuQuery);
}

double Win_Monitoring::GetCPUUsage(float deltaTime) {
    cpuTimeAccumulator += deltaTime;
    if (cpuTimeAccumulator < cpuUpdateInterval)
        return smoothCPU;

    cpuTimeAccumulator = 0.0f;

    FILETIME idleTime, kernelTime, userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime))
        return smoothCPU;

    auto filetimeToULL = [](const FILETIME& ft) -> ULONGLONG {
        return (static_cast<ULONGLONG>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
        };

    ULONGLONG idle = filetimeToULL(idleTime) - filetimeToULL(prevIdleTime);
    ULONGLONG kernel = filetimeToULL(kernelTime) - filetimeToULL(prevKernelTime);
    ULONGLONG user = filetimeToULL(userTime) - filetimeToULL(prevUserTime);

    prevIdleTime = idleTime;
    prevKernelTime = kernelTime;
    prevUserTime = userTime;

    const ULONGLONG total = kernel + user;
    if (total <= 0) return smoothCPU;
    const ULONGLONG active = total - idle;
    const ULONGLONG cpuPercentTimes100 = (active * 10000) / total;
    const double cpu = static_cast<double>(cpuPercentTimes100) / 100.0;

    if (smoothCPU == 0.0)
        smoothCPU = cpu;
    else
        smoothCPU = smoothCPU + cpuSmoothing * (cpu - smoothCPU);

    return smoothCPU;
}

FrameStats Win_Monitoring::UpdateFrameTiming(const float currentTime) {
    FrameStats stats;

    float rawDelta = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    
    if (rawDelta <= 0.0f || rawDelta > 0.5f)
        rawDelta = 0.016f;
    
    smoothDelta = smoothDelta + deltaSmoothing * (rawDelta - smoothDelta);
    stats.delta = smoothDelta;
    
    stats.fps = 1.0f / stats.delta;
    
    if (rollingAvgFps == 0.0f) {
        rollingAvgFps = stats.fps;
    } else {

        // deltaTime / 300.0f means it takes roughly 300 seconds (5 minutes)
        // for old data to completely fade out of the average.
        float alpha = stats.delta / 300.0f;
        
        // Clamp alpha just in case deltaTime spikes massively
        alpha = std::min(alpha, 1.0f);
        
        rollingAvgFps = rollingAvgFps + alpha * (stats.fps - rollingAvgFps);
    }
    
    stats.avgFps = rollingAvgFps;

    return stats;
}

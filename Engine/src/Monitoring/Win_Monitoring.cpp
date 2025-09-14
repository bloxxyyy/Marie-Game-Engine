#include "Monitoring/Win_Monitoring.h"
#include <GLFW/glfw3.h>

Win_Monitoring::Win_Monitoring() {
    glGenQueries(1, &gpuQuery);
    lastFrameTime = static_cast<float>(glfwGetTime());

    // Initialize CPU previous times
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

    ULONGLONG total = kernel + user;
    if (total <= 0) return smoothCPU;  // safeguard
    double cpu = 100.0 * (1.0 - static_cast<double>(idle) / total);

    if (smoothCPU == 0.0)
        smoothCPU = cpu;
    else
        smoothCPU = smoothCPU + cpuSmoothing * (cpu - smoothCPU);

    return smoothCPU;
}

FrameStats Win_Monitoring::UpdateFrameTiming(float currentTime) {
    FrameStats stats;

    float rawDelta = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    if (rawDelta <= 0.0f || rawDelta > 0.5f)
        rawDelta = 0.016f;

    smoothDelta = smoothDelta + deltaSmoothing * (rawDelta - smoothDelta);
    stats.deltaTime = smoothDelta;

    stats.fps = 1.0f / stats.deltaTime;
    fpsAccumulator += stats.fps;
    frameCount++;
    stats.avgFPS = fpsAccumulator / frameCount;

    return stats;
}

#pragma once

struct EngineStats {
    float deltaTime = 0.0f;
    float fps = 0.0f;
    float avgFPS = 0.0f;

    float cpuUsage = 0.0f;
    float gpuUsage = 0.0f;
    float memoryUsed = 0.0f;
    float memoryTotal = 0.0f;
};
#include "Monitoring/CpuMonitor.h"
#include <cmath>

static ULONGLONG ToUll(const FILETIME& ft) {
    return (static_cast<ULONGLONG>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

CpuMonitor::CpuMonitor() {
    FILETIME idle, kernel, user;
    GetSystemTimes(&idle, &kernel, &user);

    prevIdleTime = idle;
    prevKernelTime = kernel;
    prevUserTime = user;
}

double CpuMonitor::Update(const double deltaTime) {
    accumulator += deltaTime;

    if (accumulator < updateInterval)
        return smoothCpu;

    accumulator = 0.0;

    FILETIME idle, kernel, user;
    if (!GetSystemTimes(&idle, &kernel, &user))
        return smoothCpu;

    const ULONGLONG idleDiff   = ToUll(idle)   - ToUll(prevIdleTime);
    const ULONGLONG kernelDiff = ToUll(kernel) - ToUll(prevKernelTime);
    const ULONGLONG userDiff   = ToUll(user)   - ToUll(prevUserTime);

    prevIdleTime = idle;
    prevKernelTime = kernel;
    prevUserTime = user;

    const ULONGLONG kernelNoIdle = kernelDiff - idleDiff;
    const ULONGLONG total        = kernelNoIdle + userDiff;

    if (total == 0)
        return smoothCpu;

    const double cpu = (static_cast<double>(total) / static_cast<double>(kernelDiff + userDiff)) * 100.0;

    const double alpha = 1.0 - std::exp(-updateInterval / tau);

    if (!initialized) {
        smoothCpu = cpu;
        initialized = true;
    } else {
        smoothCpu += alpha * (cpu - smoothCpu);
    }

    return smoothCpu;
}
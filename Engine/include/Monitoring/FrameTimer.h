#pragma once
#include <vector>
#include "Data/FrameStats.h"

/**
 * @brief Calculates frame-related statistics including raw, smoothed, and average delta times/FPS.
 *
 * This class is highly optimized for real-time systems, guaranteeing O(1) update cost per frame.
 * It eliminates runtime heap allocations and uses a fixed-size circular buffer for predictable
 * memory access and cache efficiency. It provides the true, unclamped raw delta for diagnostics,
 * a smoothed delta for stable game logic, and an average FPS calculated over an approximate
 * time window, prioritizing deterministic performance for the average calculation.
 */
class FrameTimer final {
public:
    /**
     * @brief Constructs the FrameTimer with configurable smoothing and averaging parameters.
     * @param smoothingTimeConstant The time constant (tau) for the exponential moving average
     *                              of delta time. A larger value results in smoother but
     *                              less responsive delta time.
     * @param averageWindowDuration The approximate duration in seconds over which to average FPS.
     *                              This determines the initial size of the internal circular buffer.
     * @param initialTargetFps The assumed FPS for the first frame's delta calculation,
     *                         providing a stable initial value for `mSmoothDelta`.
     */
    explicit FrameTimer(
        double smoothingTimeConstant = 0.1,
        double averageWindowDuration = 5.0,
        double initialTargetFps = 60.0
    );

    /**
     * @brief Updates the timer with the current time and returns frame statistics.
     * @param currentTime The current time in seconds, typically from a high-resolution clock source.
     * @return A FrameStats struct containing the latest calculated statistics, including
     *         raw, smoothed, and average delta times and FPS.
     */
    FrameStats Update(double currentTime);

private:
    // Configuration
    const float mDeltaTau;              // Time constant (tau) for the exponential moving average, float for consistency.
    const float mWindowDuration;        // Approximate duration of the sliding window for average FPS calculation, float for consistency.
    const size_t mMaxFrameTimesCount;    // Maximum capacity of the circular buffer, pre-calculated to cover the window.

    // State
    double mLastTime;                    // Stores the timestamp of the previous frame (double for precision of absolute time).
    float mSmoothDelta;                  // The exponentially smoothed delta time.
    float mAccumulatedTime = 0.0f;       // Sum of delta times for the elements within the active window.
    bool mHasLastTime = false;           // Flag indicating if mLastTime has been set.
    
    // Fixed-size circular buffer for storing raw delta times for the sliding window average.
    std::vector<float> mFrameTimesBuffer;
    size_t mBufferHeadIndex = 0;         // Index for the next write operation into the circular buffer.
    size_t mBufferTailIndex = 0;         // Index of the oldest active element in the circular buffer.
    size_t mCurrentFrameTimesCount = 0;  // Number of elements currently considered active in the buffer.
};
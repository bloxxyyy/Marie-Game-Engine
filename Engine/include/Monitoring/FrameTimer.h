#pragma once
#include <vector>
#include "Data/FrameStats.h"

/**
 * @brief Calculates frame-related statistics like delta time and FPS.
 *
 * This class provides both a smoothed, instantaneous delta time/FPS for stable
 * game logic and a long-term average FPS calculated over a sliding time window.
 * It is designed for predictable, low-latency performance by avoiding runtime
 * heap allocations after initialization and being robust against large,
 * unpredictable time spikes.
 */
class FrameTimer final {
public:
    /**
     * @brief Constructs the FrameTimer with configurable smoothing and averaging parameters.
     * @param smoothingTimeConstant The time constant (tau) for the exponential moving average
     *                              of delta time. A larger value results in smoother but
     *                              less responsive delta time.
     * @param averageWindowDuration The duration in seconds over which to average FPS.
     *                              This determines the size of the internal circular buffer.
     * @param initialTargetFps The assumed FPS for the first frame's delta calculation,
     *                         preventing an initial delta of zero.
     */
    explicit FrameTimer(
        double smoothingTimeConstant = 0.1,
        double averageWindowDuration = 5.0,
        double initialTargetFps = 60.0
    );

    /**
     * @brief Updates the timer with the current time and returns frame statistics.
     * @param currentTime The current time in seconds, typically from a high-resolution clock.
     * @return A FrameStats struct containing the latest calculated statistics.
     */
    FrameStats Update(double currentTime);

private:
    // Configuration
    const double mDeltaTau;              // Time constant for delta time smoothing.
    const double mWindowDuration;        // Duration of the sliding window for average FPS.
    const size_t mMaxFrameTimesCount;    // Maximum capacity of the circular buffer, pre-calculated for the window.

    // State
    double mLastTime;                    // The timestamp of the previous frame, used to calculate raw delta.
    bool mHasLastTime = false;           // Flag indicating if mLastTime has been initialized.
    float mSmoothDelta;                  // The exponentially smoothed delta time.
    
    // Fixed-size circular buffer for storing raw delta times for the sliding window average.
    // This pre-allocates memory to eliminate runtime heap allocations/deallocations, ensuring predictable performance.
    std::vector<float> mFrameTimesBuffer;
    size_t mBufferHeadIndex = 0;         // Index in mFrameTimesBuffer for the next element to be written.
    size_t mBufferTailIndex = 0;         // Index in mFrameTimesBuffer for the oldest active element.
    size_t mCurrentFrameTimesCount = 0;  // Number of valid elements currently within the active window in the buffer.
    
    float mAccumulatedTime = 0.0f;       // Sum of delta times for the elements currently in the active window.
};
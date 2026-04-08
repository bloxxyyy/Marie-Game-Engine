#include "Monitoring/FrameTimer.h"
#include <algorithm> // For std::clamp, std::max
#include <cmath>     // For std::ceil

// Define a realistically high maximum expected FPS for buffer sizing.
// This ensures the circular buffer is large enough to physically hold `mWindowDuration` worth of frames
// even at very high actual frame rates (e.g., 300+ FPS), preventing premature buffer saturation
// and allowing the time-based windowing logic to operate correctly.
constexpr double max_expected_fps_for_buffer_size = 1000.0; 

FrameTimer::FrameTimer(
    const double smoothingTimeConstant,
    const double averageWindowDuration,
    const double initialTargetFps)
    // Initialize configuration members in declaration order as per header.
    : mDeltaTau(smoothingTimeConstant)
    , mWindowDuration(averageWindowDuration)
    // Calculate the maximum buffer size based on window duration and a realistic max FPS.
    // The `ceil` ensures integer buffer size. This capacity is pre-allocated.
    , mMaxFrameTimesCount(static_cast<size_t>(std::ceil(mWindowDuration * max_expected_fps_for_buffer_size)))
    // Initialize state members in declaration order.
    // `mLastTime` is initialized, but its validity is controlled by `mHasLastTime`.
    , mLastTime(0.0) 
    // `mSmoothDelta` is initialized based on the target FPS, providing a stable starting point.
    // Cast to `float` as `mSmoothDelta` is `float`.
    , mSmoothDelta(static_cast<float>(1.0 / std::max(initialTargetFps, 0.0001))) 
    // Pre-allocate the vector to its maximum capacity. This ensures no runtime heap allocations.
    , mFrameTimesBuffer(mMaxFrameTimesCount)
{
}

FrameStats FrameTimer::Update(const double currentTime) {

    if (!mHasLastTime) {
        mLastTime = currentTime;
        mHasLastTime = true;
        return {}; 
    }
    
    double rawDeltaDouble = currentTime - mLastTime; 
    mLastTime = currentTime;

    // Clamp the raw delta time to a sensible range (`[0.0001, 0.5]` seconds).
    // This is crucial to prevent "spiral of death" scenarios or physics explosions caused by
    // extremely large delta times (e.g., from debugger pauses, heavy loading, or OS scheduling delays).
    rawDeltaDouble = std::clamp(rawDeltaDouble, 0.0001, 0.5);
    
    // Convert to `float` for internal storage and calculations, as `float` precision is sufficient.
    const float rawDelta = static_cast<float>(rawDeltaDouble); 

    // --- Calculate Smoothed Delta and FPS ---
    // A frame-rate independent exponential moving average (EMA) is used to smooth delta time,
    // providing a more stable delta for game logic and reducing jitter effects.
    // The alpha coefficient uses a fast, stable approximation: `dt / (tau + dt)`.
    // This replaces an expensive `exp()` call with simpler arithmetic, significantly improving throughput.
    // Alpha is calculated with `double` precision for better accuracy before casting to `float`.
    const float alpha = static_cast<float>(rawDeltaDouble / (mDeltaTau + rawDeltaDouble)); 
    mSmoothDelta += alpha * (rawDelta - mSmoothDelta);
    
    // No `std::max` is needed here because `rawDelta` is clamped, ensuring `mSmoothDelta` will always be positive.
    const float smoothedFps = 1.0f / mSmoothDelta; 
    
    // If the buffer is physically full (`mCurrentFrameTimesCount == mMaxFrameTimesCount`),
    // adding a new element will overwrite the oldest element.
    // In this case it should subtracts the value of the overwritten element from `mAccumulatedTime`
    // and advance `mBufferTailIndex` to maintain the sum and track the new oldest element.
    if (mCurrentFrameTimesCount == mMaxFrameTimesCount) {
        mAccumulatedTime -= mFrameTimesBuffer[mBufferHeadIndex];
        // Advance the physical tail index as an element is conceptually removed by overwrite.
        if (++mBufferTailIndex == mMaxFrameTimesCount) {
            mBufferTailIndex = 0;
        }
    } else {
        // If the buffer is not yet full, simply increment the count of valid elements.
        mCurrentFrameTimesCount++;
    }
    
    // Store the new raw delta time in the circular buffer at the head index.
    mFrameTimesBuffer[mBufferHeadIndex] = rawDelta;
    mAccumulatedTime += rawDelta;

    // Advance the head index, wrapping around when it reaches the end of the buffer.
    if (++mBufferHeadIndex == mMaxFrameTimesCount) {
        mBufferHeadIndex = 0;
    }

    // Remove old frame times from the logical "front" (tail) of the circular buffer.
    // This `while` loop should enforce the `mWindowDuration` by continuously removing the oldest logical elements
    // until `mAccumulatedTime` is within the desired window.
    while (mAccumulatedTime > mWindowDuration && mCurrentFrameTimesCount > 0) {
        mAccumulatedTime -= mFrameTimesBuffer[mBufferTailIndex];
        
        // Advance the tail index, wrapping around.
        // This advances the logical window's tail, independent of physical buffer writes.
        if (++mBufferTailIndex == mMaxFrameTimesCount) {
            mBufferTailIndex = 0;
        }
        mCurrentFrameTimesCount--; // Logically remove the oldest element from consideration.
    }

    float averageFps = smoothedFps;
    if (mCurrentFrameTimesCount > 0) {
        // Calculate average delta using only the currently active elements in the buffer.
        // `static_cast` ensures explicit conversion from `size_t` to `float`, preventing warnings.
        const float avgDelta = mAccumulatedTime / static_cast<float>(mCurrentFrameTimesCount);
        // No `std::max` is needed here because `mAccumulatedTime`should always be positive.
        averageFps = 1.0f / avgDelta; 
    }
    
    return {.delta = mSmoothDelta, .fps = smoothedFps, .avgFps = averageFps};
}
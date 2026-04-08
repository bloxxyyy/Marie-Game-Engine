#include "Monitoring/FrameTimer.h"
#include <algorithm>
#include <cmath>

// Define a high but realistic maximum expected FPS for buffer sizing.
// This ensures the circular buffer can physically hold `mWindowDuration` worth of frames
// even at very high actual frame rates (e.g., 500+ FPS), preventing premature buffer saturation.
constexpr double max_expected_fps_for_buffer_size = 1000.0; 

FrameTimer::FrameTimer(
    const double smoothingTimeConstant,
    const double averageWindowDuration,
    const double initialTargetFps)
    // Initialize configuration members in declaration order.
    // Cast `double` constructor arguments to `float` for `mDeltaTau` and `mWindowDuration`
    // to maintain type consistency within the hot path and improve performance.
    : mDeltaTau(static_cast<float>(smoothingTimeConstant))
    , mWindowDuration(static_cast<float>(averageWindowDuration))
    // Calculate the maximum buffer size based on window duration and a realistic max FPS.
    // `ceil` ensures integer buffer size. This capacity is pre-allocated.
    , mMaxFrameTimesCount(static_cast<size_t>(std::ceil(averageWindowDuration * max_expected_fps_for_buffer_size)))
    // Initialize state members in declaration order for memory packing.
    // `mLastTime` is explicitly initialized; its validity is controlled by `mHasLastTime`.
    , mLastTime(0.0) 
    // Initializes `mSmoothDelta` based on the target FPS to provide a stable starting value.
    , mSmoothDelta(static_cast<float>(1.0 / std::max(initialTargetFps, 0.0001))) 
    // Pre-allocate the vector to its maximum capacity. This ensures no runtime heap allocations
    // after initialization. Elements are value-initialized to 0.0f.
    , mFrameTimesBuffer(mMaxFrameTimesCount)
{
}

FrameStats FrameTimer::Update(const double currentTime) {

    if (!mHasLastTime) {
        mLastTime = currentTime;
        mHasLastTime = true;
        return {}; 
    }

    // Calculate the raw delta time since the last update using `double` for maximum precision.
    double rawDeltaDouble = currentTime - mLastTime; 
    mLastTime = currentTime;

    // Preserve the original, unclamped raw delta for diagnostic purposes.
    const float unclampedRawDelta = static_cast<float>(rawDeltaDouble);

    // Clamp the raw delta time to a sensible range (`[0.0001, 0.5]` seconds) for internal logic.
    // This prevents "spiral of death" scenarios or physics explosions caused by
    // extremely large delta times (e.g., from debugger pauses, heavy loading, or OS scheduling delays).
    rawDeltaDouble = std::clamp(rawDeltaDouble, 0.0001, 0.5);
    // Convert to `float` for internal storage and calculations, as `float` precision is sufficient for game engine delta times.
    const float clampedDelta = static_cast<float>(rawDeltaDouble); 

    // --- Calculate Smoothed Delta and FPS ---
    // A frame-rate independent exponential moving average (EMA) is used to smooth delta time,
    const float alpha = static_cast<float>(rawDeltaDouble / (mDeltaTau + rawDeltaDouble)); 
    mSmoothDelta += alpha * (clampedDelta - mSmoothDelta);
    
    // `mSmoothDelta` is guaranteed to be positive due to clamping, so no `std::max` is needed.
    const float smoothedFps = 1.0f / mSmoothDelta; 
    
    // --- Update Circular Buffer for Average FPS Calculation ---
    // If the buffer is full, the oldest element's value is removed from `mAccumulatedTime`,
    // and the `mBufferTailIndex` is advanced to reflect the overwrite, ensuring `mAccumulatedTime` remains accurate.
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
    
    // Store the new clamped delta time in the circular buffer at the head index.
    mFrameTimesBuffer[mBufferHeadIndex] = clampedDelta;
    mAccumulatedTime += clampedDelta;

    // Advance the head index, wrapping around when it reaches the end of the buffer.
    // This uses a branch instead of a modulo operator for improved performance and predictability.
    if (++mBufferHeadIndex == mMaxFrameTimesCount) {
        mBufferHeadIndex = 0;
    }
    
    if (mAccumulatedTime > mWindowDuration && mCurrentFrameTimesCount > 0) {
        mAccumulatedTime -= mFrameTimesBuffer[mBufferTailIndex];
        
        // Advance the tail index, wrapping around.
        // This advances the logical window's tail, independent of physical buffer writes.
        if (++mBufferTailIndex == mMaxFrameTimesCount) {
            mBufferTailIndex = 0;
        }
        mCurrentFrameTimesCount--; 
    }

    float averageFps = smoothedFps; // Default to smoothed FPS if the average buffer is empty (e.g., at startup).
    if (mCurrentFrameTimesCount > 0) {
        // Calculate average delta using only the currently active elements in the buffer.
        // `static_cast` ensures explicit conversion from `size_t` to `float`, preventing warnings.
        const float avgDelta = mAccumulatedTime / static_cast<float>(mCurrentFrameTimesCount);
        averageFps = 1.0f / avgDelta; 
    }
    
    return {.rawDelta = unclampedRawDelta, .delta = mSmoothDelta, .fps = smoothedFps, .avgFps = averageFps};
}
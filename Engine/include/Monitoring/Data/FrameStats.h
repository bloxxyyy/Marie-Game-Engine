#pragma once

struct FrameStats {
    float rawDelta = 0.0f; // The true, unclamped delta time, useful for diagnostics and spike detection.
    float delta = 0.0f;    // The clamped and smoothed delta time, intended for stable game logic updates.
    float fps = 0.0f;      // Instantaneous FPS derived from the smoothed delta.
    float avgFps = 0.0f;   // Average FPS over the defined sliding time window.
};
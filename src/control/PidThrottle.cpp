// // Member variables you’ll need in ControlUnit.h (or similar)
// mutable float currentThrottle = 0.0f;
// mutable float throttleIntegral = 0.0f;
// mutable float prevThrottleError = 0.0f;
// mutable unsigned long prevTimeMicros = 0;
//
// // PID gains – tune these!
// const float Kp = 2.0f;
// const float Ki = 0.5f;
// const float Kd = 0.1f;
//
// void ControlUnit::updateThrottleState(const float throttleValue, const float brakeValue) const {
//     const float deadzone = 0.05f;
//     const float brakeThreshold = 0.5f;
//
//     // Time delta in seconds
//     unsigned long now = micros();
//     float dt = (prevTimeMicros == 0) ? 0.01f : (now - prevTimeMicros) / 1e6f;
//     prevTimeMicros = now;
//
//     // Handle braking
//     if (brakeValue > brakeThreshold) {
//         throttleIntegral = 0;
//         prevThrottleError = 0;
//         currentThrottle = 0;
//         vescUnit.setDuty(0);
//         vescUnit.setBrakeCurrent(driveMode.brakeForce);
//         return;
//     }
//
//     // Compute targetThrottle from input
//     float targetThrottle = 0;
//     if (fabs(throttleValue) >= deadzone) {
//         if (throttleValue > 0) {
//             targetThrottle = throttleValue * driveMode.maxPower;
//         } else {
//             targetThrottle = throttleValue * driveMode.reversePower;
//         }
//     }
//
//     // PID calculations
//     float error = targetThrottle - currentThrottle;
//     throttleIntegral += error * dt;
//     float derivative = (error - prevThrottleError) / dt;
//     prevThrottleError = error;
//
//     float output = (Kp * error) + (Ki * throttleIntegral) + (Kd * derivative);
//
//     currentThrottle += output * dt;
//
//     // Safety clamp
//     currentThrottle = std::clamp(currentThrottle, -1.0f, 1.0f);
//
//     // Send to VESC
//     vescUnit.setDuty(currentThrottle);
// }

// Instead of a fixed maxDelta, a PID controller calculates the required change in throttle based on:
// - How far you are from the target (P)
// - How long you've been off-target (I)
// - How quickly you're approaching the target (D)
// This helps avoid sudden jerks and gives a fluid throttle response — and works in both forward and reverse.

// Tuning Tips
// Start with only P (Kp). Keep Ki and Kd = 0. Slowly increase Kp until it feels responsive but not twitchy.
// Add a little I (Ki) if there's slow convergence to target (lag).
// Add D (Kd) if you get overshoot or oscillation.
//
// Start values:
// const float Kp = 2.0f;
// const float Ki = 0.5f;
// const float Kd = 0.1f;
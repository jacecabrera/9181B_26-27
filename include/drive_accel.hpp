#pragma once
#include <cmath>
#include <algorithm>

namespace b_template {
    namespace drive_accel {

        // Constants

        // Both the joystick and move() top out at 127.
        inline constexpr double FULL_COMMAND = 127.0;

        // All rates below are fractions of full power per second.
        // 2.0 means full power takes 0.5s 
        //etc.  5.0-->0.2 s  
        //     10.0-->0.1 s.

        // When stick is just past the deadband
        inline constexpr double LIGHT_PUSH_RATE = 0.5;

        // Speed-up when the stick is fully pushed
        inline constexpr double FULL_PUSH_RATE = 2.0;

        // Slow down is instant at or below this fraction of full power.
        inline constexpr double HIGH_SPEED_START_FRACTION = 0.75;

        // Fastest allowed drop above that fraction.
        inline constexpr double HIGH_SPEED_BRAKE_RATE = 6.0;

        // Longest time step the ramp will use. 
        inline constexpr double LONGEST_STEP_SECONDS = 0.05;

        // Functions

        // Converts a raw stick value into the command it is asking for
        // The output starts at 0 at the deadband edge and reaches full at full stick
        inline double stick_to_command(int raw_stick, double deadband) {
            double stick_distance = std::fabs(static_cast<double>(raw_stick));
            if (stick_distance <= deadband) {
                return 0.0;
            }
            double push_fraction = (stick_distance - deadband) / (FULL_COMMAND - deadband);
            return std::copysign(push_fraction * FULL_COMMAND, static_cast<double>(raw_stick));
        }

        // Moves the previous forward command toward what the stick asks for
        // Call once per loop and store the result
        inline double ramp_forward_command(int raw_stick,
                                           double previous_command,
                                           double seconds_since_last_update,
                                           double deadband) {
            double step_seconds = std::min(seconds_since_last_update, LONGEST_STEP_SECONDS);
            double target_command = stick_to_command(raw_stick, deadband);

            double previous_size = std::fabs(previous_command);
            double target_size   = std::fabs(target_command);
            bool slowing_down = (target_size < previous_size) &&
                                (target_command * previous_command >= 0.0);

            if (slowing_down) {
                // Low and medium speeds: drop straight to the target
                if (previous_size <= HIGH_SPEED_START_FRACTION * FULL_COMMAND) {
                    return target_command;
                }
                // High speed: limit the drop this step
                double largest_drop = HIGH_SPEED_BRAKE_RATE * FULL_COMMAND * step_seconds;
                double new_size = std::max(target_size, previous_size - largest_drop);
                return std::copysign(new_size, previous_command);
            }

            // Speeding up or reversing: rate grows exponentially with stick position
            double push_fraction = target_size / FULL_COMMAND;
            double rate = LIGHT_PUSH_RATE * std::pow(FULL_PUSH_RATE / LIGHT_PUSH_RATE, push_fraction);
            double largest_change = rate * FULL_COMMAND * step_seconds;

            return previous_command +
                   std::clamp(target_command - previous_command, -largest_change, largest_change);
        }
    }
}
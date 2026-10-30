#ifndef B_TEMPLATE_SUBSYSTEM_LIFT_HPP
#define B_TEMPLATE_SUBSYSTEM_LIFT_HPP

#include "b_template/util/pidf.hpp"
#include "main.h"

namespace b_template {
namespace subsystem {

class Lift {
public:
    Lift();

    /**
     * @brief Nudges the target position up or down for manual control.
     * @param delta Amount of ticks to add/subtract (positive = up, negative = down)
     */
    void jog(double delta);

    /**
     * @brief Sets an absolute target height preset.
     */
    void set_target(double target);

    /**
     * @brief Computes PIDF and applies voltage to motors. Must be called every loop.
     */
    void update();

    double get_position() const;
    double get_target() const;

private:
    pros::MotorGroup lift_motors;
    utils::PIDF lift_pidf;
    double current_target;
    
    const double max_height = 1200.0; // Upper mechanical limit
    const double min_height = 0.0;    // Ground limit
};

} // namespace subsystem
} // namespace b_template

#endif // B_TEMPLATE_SUBSYSTEM_LIFT_HPP
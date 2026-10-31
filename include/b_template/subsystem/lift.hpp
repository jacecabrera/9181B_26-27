#ifndef B_TEMPLATE_SUBSYSTEM_LIFT_HPP
#define B_TEMPLATE_SUBSYSTEM_LIFT_HPP

#include "b_template/util/pidf.hpp"
#include "main.h"
#include <memory>

namespace b_template {
namespace subsystem {

class Lift {
public:
    Lift();
    void init();                       // call once from initialize(), lift fully down
    void jog(double delta);
    void hold();                       // freeze target at current position
    void set_target(double target);
    void update();                     // run by the internal task
    double get_position() const;
    double get_target() const;
    bool at_target(double tolerance = 15.0) const;

private:
    double position() const;

    pros::MotorGroup lift_motors;
    utils::PIDF lift_pidf;
    mutable pros::Mutex mtx;
    std::unique_ptr<pros::Task> task_;
    double current_target = 0.0;

    static constexpr double max_height = 1200.0;
    static constexpr double min_height = 0.0;
    static constexpr double max_lead   = 120.0;
    static constexpr double rest_band  = 25.0;
};

} // namespace subsystem
} // namespace b_template

#endif
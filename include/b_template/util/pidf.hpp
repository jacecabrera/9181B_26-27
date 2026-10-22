#ifndef B_TEMPLATE_UTILS_PIDF_HPP
#define B_TEMPLATE_UTILS_PIDF_HPP

namespace b_template {
namespace utils {

class PIDF {
public:
    /**
     * @param kP Proportional gain
     * @param kI Integral gain
     * @param kD Derivative gain
     * @param kF Velocity feedforward gain (optional, defaults to 0)
     * @param kG Gravity feedforward constant (optional, defaults to 0)
     * @param starti Error threshold below which integral accumulates
     * @param max_i Maximum limit for accumulated integral
     */
    PIDF(double kP, double kI, double kD, double kF = 0.0, double kG = 0.0, double starti = 0.0, double max_i = 0.0);

    /**
     * @brief Sets target position and resets state.
     */
    void set_target(double target);

    /**
     * @brief Computes control output using optional target velocity for velocity feedforward.
     * @param current Current measured position or velocity.
     * @param target_vel Planned target velocity (used with kF). Defaults to 0.0.
     * @return Calculated output in motor units (e.g., mV).
     */
    double compute(double current, double target_vel = 0.0);

    /**
     * @brief Resets accumulated error, integral, and derivative states.
     */
    void reset();

    /**
     * @brief Updates constants dynamically.
     */
    void set_constants(double kP, double kI, double kD, double kF = 0.0, double kG = 0.0, double starti = 0.0, double max_i = 0.0);

    // Getters
    double get_target() const;
    double get_error() const;

private:
    double kP, kI, kD, kF, kG;
    double starti, max_i;

    double target = 0.0;
    double error = 0.0;
    double prev_error = 0.0;
    double integral = 0.0;
    double derivative = 0.0;
};

} // namespace utils
} // namespace b_template

#endif // B_TEMPLATE_UTILS_PIDF_HPP
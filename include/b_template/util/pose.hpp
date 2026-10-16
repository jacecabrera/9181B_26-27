#pragma once

namespace b_template {
    struct Pose {
        double x = 0.0;
        double y = 0.0;
        double theta = 0.0;

        Pose() = default;
        Pose(double x, double y, double theta = 0.0);

        double distance_to(const Pose& target) const;
        double angle_to(const Pose& target) const;
        
        Pose rotate(double angle) const;
    };
}

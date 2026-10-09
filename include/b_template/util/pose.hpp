#pragma once

#include <cmath>

namespace b_template {
    stuct Pose{
        double x = {0,0};
        double y = {0,0};
        double theta = {0,0};

        void Pose() = default;
        Pose(double x, double y, theta = 0.0);

        double distance_to(const Pose& target);
        double angle_to(const Pose& target);
        
        Pose rotate(double angle) const;
    }
}

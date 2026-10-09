#include <numbers>

namespace b_template {
    namespace math {
        //conversion utilites

        inline constexpr = deg_to_rad(double deg) {
            //multiply the inputted degree by (pi/180)
            return deg * (std::numbers::pi / 180.0); // 180.0 instead of 180 to avoid integer truncation (to be safe)
        }

        //opposite of top function
        inline constexpr = rad_to_deg(double rad) {
            return deg * (180.0 / std::numbers::pi);
        }
    }
}
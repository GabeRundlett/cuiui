#pragma once
#include <glm/glm.hpp>

namespace cuiui {
    struct constraint {
        struct procedure {
            float value;
            float (*proc)(float, float);
            float operator()(float x) {
                return proc(x, value);
            }
        };
        std::vector<procedure> procedures;
        float value;

        inline constraint operator+(const constraint &other) {
            return *this;
        }
        inline constraint operator-(const constraint &other) {
            return *this;
        }
        inline constraint operator+=(const constraint &other) {
            return *this;
        }
        inline constraint operator-=(const constraint &other) {
            return *this;
        }
    };

    inline constraint pixel_constraint(float x) {
        return {
            .procedures = {{
                .value = x,
                .proc = [](float x, float val) {
                    return x; // TODO: fix math
                },
            }},
        };
    }
    inline constraint percent_constraint(float x) {
        return {
            .procedures = {{
                .value = x,
                .proc = [](float x, float val) {
                    return x; // TODO: fix math
                },
            }},
        };
    }

    namespace constraint_literals {
        inline constraint operator""_px(unsigned long long x) { return pixel_constraint(x); }
        inline constraint operator""_px(long double x) { return pixel_constraint(x); }

        inline constraint operator""_pc(unsigned long long x) { return percent_constraint(x); }
        inline constraint operator""_pc(long double x) { return percent_constraint(x); }
    } // namespace constraint_literals

    struct constraint2d {
        constraint x, y;

        // operator(glm::ivec2)() {
        //     return {int(x.value), int(y.value)};
        // }
    };
} // namespace cuiui

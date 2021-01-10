#pragma once
#include <glm/glm.hpp>

namespace cuiui {
    struct constraint {
        using procedure = constraint (*)(float);
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
        return {.procedures = {[=](float val) { return x; }}, .value = x};
    }
    inline constraint percent_constraint(float x) {
        return {.value = x};
    }

    namespace constraint_literals {
        inline constraint operator""_px(unsigned long long x) { return pixel_constraint(x); }
        inline constraint operator""_px(long double x) { return pixel_constraint(x); }

        inline constraint operator""_pc(unsigned long long x) { return percent_constraint(x); }
        inline constraint operator""_pc(long double x) { return percent_constraint(x); }
    } // namespace constraint_literals

    struct constraint2d {
        constraint x, y;

        inline operator(glm::ivec2)() {
            return {int(x.value), int(y.value)};
        }
    };
} // namespace cuiui

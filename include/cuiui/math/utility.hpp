#pragma once

#include <cstdint>
#include <numbers>

#include "types.hpp"

constexpr auto radians(auto angle_degrees) {
    return angle_degrees * std::numbers::pi_v<decltype(angle_degrees)> / 180;
}

constexpr u32 get_hovered_edge(i32rect r, i32vec2 p, int margin) {
    u32 result = 0;
    p -= r.p0;
    auto size = r.p1 - r.p0;
    if (p.x < 0 || p.x > size.x || p.y < 0 || p.y > size.y)
        return 0xffffffff;
    if (p.x < margin) {
        result |= 0x01;
    } else if (p.x >= size.x - margin) {
        result |= 0x02;
    }
    if (p.y < margin) {
        result |= 0x04;
    } else if (p.y >= size.y - margin) {
        result |= 0x08;
    }
    return result;
}

constexpr void drag(u32 grab_mode, i32vec2 &pos, i32vec2 &dim, i32vec2 min_dim, i32vec2 currentpos, i32vec2 mouse_down_point) {
    if (grab_mode) {
        if (grab_mode & 0x01) {
            // resize x left
            dim.x = pos.x + dim.x - (currentpos.x - mouse_down_point.x);
            i32 oversize_offset = 0;
            if (dim.x < min_dim.x) {
                oversize_offset = min_dim.x - dim.x;
                dim.x = min_dim.x;
            }
            pos.x = currentpos.x - mouse_down_point.x - oversize_offset;
        } else if (grab_mode & 0x02) {
            // resize x right
            dim.x = currentpos.x - pos.x + mouse_down_point.x;
            if (dim.x < min_dim.x)
                dim.x = min_dim.x;
        }
        if (grab_mode & 0x04) {
            // resize y top
            dim.y = pos.y + dim.y - (currentpos.y - mouse_down_point.y);
            i32 oversize_offset = 0;
            if (dim.y < min_dim.y) {
                oversize_offset = min_dim.y - dim.y;
                dim.y = min_dim.y;
            }
            pos.y = currentpos.y - mouse_down_point.y - oversize_offset;
        } else if (grab_mode & 0x08) {
            // resize y bottom
            dim.y = currentpos.y - pos.y + mouse_down_point.y;
            if (dim.y < min_dim.y)
                dim.y = min_dim.y;
        }
    } else {
        // move
        pos = currentpos - mouse_down_point;
    }
}

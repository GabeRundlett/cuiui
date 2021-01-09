#include <cuiui/ui_components.hpp>
#include <iostream>

#include <gl/GL.h>
#include <gl/GLU.h>

namespace cuiui {
    button::button(ui_container &parent, const configuration &config)
        : internal_state{.parent = parent, .config = config} {}
    void button::on_event(const coel::window_event &event) {
        switch (event.native.message_id) {
        case WM_LBUTTONDOWN:
            if (internal_state.is_hovered) {
                internal_state.is_pressed = true;
                if (internal_state.config.on_press)
                    internal_state.config.on_press(event.parent_app, event.parent_window, *this);
            }
            break;
        case WM_LBUTTONUP:
            if (internal_state.is_pressed) {
                internal_state.is_pressed = false;
                if (internal_state.config.on_release)
                    internal_state.config.on_release(event.parent_app, event.parent_window, *this);
            }
            break;
        case WM_MOUSEMOVE: {
            glm::ivec2 m = {LOWORD(event.native.lparam), HIWORD(event.native.lparam)};
            glm::ivec2 p1 = internal_state.config.pos, p2 = internal_state.config.pos + internal_state.config.dim;
            internal_state.is_hovered = m.x >= p1.x && m.x < p2.x && m.y >= p1.y && m.y < p2.y;
        } break;
        }
    }
    void button::on_paint(coel::renderer &r) {
        glBegin(GL_QUADS);

        glm::vec3 col = {0.8, 0.8, 1};
        glColor3f(col.r, col.g, col.b);

        glm::vec2 p1 = glm::vec2(internal_state.config.pos) / glm::vec2(r.frame_dim) * glm::vec2(2, 2) - glm::vec2(1, 1);
        glm::vec2 p2 = p1 + glm::vec2(internal_state.config.dim) / glm::vec2(r.frame_dim) * glm::vec2(2, 2);
        p1.y = -p1.y;
        p2.y = -p2.y;

        glVertex2f(p1.x, p1.y);
        glVertex2f(p1.x, p2.y);
        glVertex2f(p2.x, p2.y);
        glVertex2f(p2.x, p1.y);

        glEnd();
        glFlush();
    }
    void button::on_resize() {
    }

    slider::slider(ui_container &parent, const configuration &config)
        : internal_state{.parent = parent, .config = config} {}
    void slider::on_event(const coel::window_event &event) {
    }
    void slider::on_paint(coel::renderer &r) {
    }
    void slider::on_resize() {
    }
} // namespace cuiui

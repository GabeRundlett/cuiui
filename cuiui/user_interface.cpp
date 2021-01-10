#include <cuiui/user_interface.hpp>

namespace cuiui {
    ui_container::ui_container(ui_container &parent) : parent(parent) {}

    void ui_container::on_event(const coel::window_event &window_event) {
        for (auto &component : components)
            component->on_event(window_event);
    }

    void ui_container::on_paint(coel::renderer &r) {
        for (auto &component : components)
            component->on_paint(r);
    }

    void ui_container::on_resize() {
        for (auto &component : components)
            component->on_resize();
    }
} // namespace cuiui

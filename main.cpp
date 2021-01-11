#include <coel/core.hpp>
#include <cuiui/core.hpp>

#include <exception>

#include <gl/GL.h>
#include <gl/GLU.h>

using namespace cuiui::constraint_literals;

class my_app : public coel::application {
  public:
};

class my_window : public coel::window, public cuiui::ui_container {
    coel::renderer renderer;

  public:
    my_window() : coel::window("My Window", {800, 600}), ui_container(*this), renderer(*this) {
        add_element<cuiui::button>(cuiui::button::configuration{
            .position_constraint = {10_px, 10_px},
            .dimension_constraint = {200_px, 100_pc - 20_px},
            .on_release = [](coel::application &parent_app, coel::window &parent_window, cuiui::button &button) {
                auto &app = static_cast<my_app &>(parent_app);
                app.add_window<my_window>();
            },
        });
    }

    void on_event(const coel::window_event &window_event) override {
        renderer.on_event(window_event);
        switch (window_event.native.message_id) {
        case WM_PAINT:
            glClear(GL_COLOR_BUFFER_BIT);
            ui_container::on_paint(renderer);
            break;
        case WM_SIZE:
            dim = {
                cuiui::pixel_constraint(LOWORD(window_event.native.lparam)),
                cuiui::pixel_constraint(HIWORD(window_event.native.lparam)),
            };
            ui_container::on_resize();
            break;
        default:
            ui_container::on_event(window_event);
            break;
        }
    }
};

int main() {
    try {
        my_app app;
        app.add_window<my_window>();
        while (!app.windows.empty())
            app.update();
    } catch (std::exception &e) {
        MessageBox(nullptr, e.what(), "std::exception caught", MB_OK);
        return -1;
    } catch (...) {
        MessageBox(nullptr, "Program crashed due to an unknown exception", "Bruh, epic fail!", MB_OK);
        return -1;
    }
}

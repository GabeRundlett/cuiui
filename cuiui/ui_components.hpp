#pragma once
#include <cuiui/user_interface.hpp>

#include <coel/application.hpp>
#include <coel/renderer.hpp>

namespace cuiui {
    class button : public ui_component {
      public:
        using interact_callback = void (*)(coel::application &, coel::window &, button &);
        struct configuration {
            glm::ivec2 pos, dim;
            interact_callback on_press, on_release;
        };

      private:
        struct {
            ui_container &parent;
            configuration config;
            std::uint32_t is_hovered : 1, is_pressed : 1;
        } internal_state;

      public:
        button(ui_container &parent, const configuration &config);
        void on_event(const coel::window_event &e) override;
        void on_paint(coel::renderer &r) override;
        void on_resize() override;
    };

    class slider : public ui_component {
      public:
        using interact_callback = void (*)(coel::application &, coel::window &, slider &);
        struct configuration {
            interact_callback on_press, on_release, on_drag;
        };

      private:
        struct {
            ui_container &parent;
            configuration config;
            std::uint32_t is_hovered : 1, is_grabbed : 1;
            float normalized_value = 0.0f;
        } internal_state;

      public:
        slider(ui_container &parent, const configuration &config);
        void on_event(const coel::window_event &e) override;
        void on_paint(coel::renderer &r) override;
        void on_resize() override;
    };
} // namespace cuiui

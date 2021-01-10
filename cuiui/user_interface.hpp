#pragma once
#include <vector>
#include <memory>

#include <coel/window.hpp>
#include <coel/renderer.hpp>
#include <glm/glm.hpp>

#include <cuiui/constraint.hpp>

namespace cuiui {
    class ui_component {
      public:
        virtual void on_event(const coel::window_event &) = 0;
        virtual void on_paint(coel::renderer &) = 0;
        virtual void on_resize() = 0;
    };

    class ui_container : ui_component {
        std::vector<std::unique_ptr<ui_component>> components;

      public:
        constraint2d pos, dim;
        ui_container &parent;

        ui_container(ui_container &parent);

        template <typename component_t, typename... ctor_args_t>
        component_t &add_element(ctor_args_t... ctor_args) {
            components.push_back(std::make_unique<component_t>(*this, ctor_args...));
            return static_cast<component_t &>(*components.back());
        }

        void on_event(const coel::window_event &e) override;
        void on_paint(coel::renderer &r) override;
        void on_resize() override;
    };
} // namespace cuiui

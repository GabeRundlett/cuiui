#pragma once

#include <cuiui/cuiui.hpp>

namespace cuiui::platform::null {
    struct CUIUI_EXPORT Window : cuiui::WindowState {
        using WindowHandleType = cuiui::WindowHandle<Window>;
        void create(const WindowConfig &config);
        void destroy();
        void update();
    };

    struct CUIUI_EXPORT Context : public cuiui::Context<Window> {
        using WindowHandleType = cuiui::WindowHandle<Window>;
        Context();
        ~Context();
        void update();
    };
} // namespace cuiui::platform::null

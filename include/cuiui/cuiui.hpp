#pragma once

#include <unordered_map>
#include <string_view>
#include <memory>

namespace cuiui {
    struct WindowConfig {
        std::string_view id;
        uint32_t size_x, size_y;
    };

    struct WindowNativeData;

    struct Window {
        WindowNativeData *native_ptr;
        bool should_close;

        uint32_t size_x, size_y;

        void CUIUI_EXPORT bind_render_ctx();
        void CUIUI_EXPORT present();
    };

    using WindowHandle = std::shared_ptr<Window>;

    class CUIUI_EXPORT Context {
        std::unordered_map<std::string_view, WindowHandle> windows;

      public:
        Context();
        ~Context();

        Context(const Context &) = delete;
        Context(Context &&) = delete;
        Context &operator=(const Context &) = delete;
        Context &operator=(Context &&) = delete;

        WindowHandle window(const WindowConfig &config);
        void update();
    };
} // namespace cuiui

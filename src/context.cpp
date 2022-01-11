#include <cuiui/cuiui.hpp>
#include "platform.hpp"

namespace cuiui {
    Context::Context() {
        platform::context_init();
    }

    Context::~Context() {
        for (auto &[key, window_handle] : windows)
            platform::context_window_deinit(window_handle);
        platform::context_deinit();
    }

    WindowHandle Context::window(const WindowConfig &config) {
        auto find_window = windows.find(config.id);
        if (find_window != windows.end()) {
            platform::context_window_update(find_window->second);
            return find_window->second;
        }
        auto &result = (windows[config.id] = std::make_shared<Window>(Window{
                            .native_ptr = nullptr,
                            .should_close = false,
                        }));
        platform::context_window_init(result, config);
        return result;
    }

    void Context::update() {
        platform::context_update();
    }
} // namespace cuiui

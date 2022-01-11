#pragma once
#include <cuiui/cuiui.hpp>

namespace cuiui::platform {
    void context_init();
    void context_deinit();
    void context_update();
    void context_window_init(WindowHandle &window_handle, const WindowConfig &config);
    void context_window_deinit(WindowHandle &window_handle);
    void context_window_update(WindowHandle &window_handle);
} // namespace cuiui

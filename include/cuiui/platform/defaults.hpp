#pragma once

#include <cuiui/platform/x11.hpp>
#include <cuiui/platform/win32.hpp>

namespace cuiui::platform::defaults {
#if CUIUI_USE_WIN32
    using Window = win32::Window; // Borderless;
    using Context = win32::Context<Window>;
#endif
#if CUIUI_USE_X11
    using Window = x11::Window;
    using Context = x11::Context;
#endif
} // namespace cuiui::platform::defaults

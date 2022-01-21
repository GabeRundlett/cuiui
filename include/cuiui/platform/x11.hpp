#pragma once

#if CUIUI_USE_X11
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/Xresource.h>
#include <X11/extensions/Xrandr.h>
#include <GL/glx.h>
#include <sys/time.h>
#include <stdexcept>

#include <cuiui/cuiui.hpp>
#include <utility>
#include <iostream>
#include <cuiui/math/utility.hpp>

namespace cuiui::platform::x11 {
    struct Window : cuiui::Window {
        using WindowHandleType = cuiui::WindowHandle<Window>;

        Display *display;
        ::Window xid;
        Atom wm_delete_window;
        bool should_close = false;

        void CUIUI_EXPORT create(const WindowConfig &config);
        void CUIUI_EXPORT destroy();
        void CUIUI_EXPORT update();
    };

    template <typename WindowT>
    struct Context : public cuiui::Context<WindowT> {
        using WindowHandleType = cuiui::WindowHandle<WindowT>;
        static inline size_t ctx_count = 0;
        Context() {
            if (ctx_count > 0)
                return;
            XInitThreads();
            XSetErrorHandler([](::Display *d, ::XErrorEvent *e) -> int {
                char buffer[512];
                XGetErrorText(d, e->type, buffer, 512);
                throw std::runtime_error(buffer);
                return 0;
            });
            ctx_count++;
        }
        ~Context() {
            ctx_count--;
            if (ctx_count > 0)
                return;
        }
        void update() {
        }
    };
} // namespace cuiui::platform::x11
#endif

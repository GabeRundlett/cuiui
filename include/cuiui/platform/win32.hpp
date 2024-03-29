#pragma once

#if CUIUI_USE_WIN32
#include <Windows.h>

#include <cuiui/cuiui.hpp>
#include <dwmapi.h>
#include <cuiui/math/utility.hpp>

namespace cuiui::platform::win32 {
    struct Window : cuiui::WindowState {
        using WindowHandleType = cuiui::WindowHandle<Window>;
        static constexpr const char *window_class_name = "cuiuiwc_win32";
        HWND hwnd;
        f32vec2 mouse_pos;

        void CUIUI_EXPORT create(const WindowConfig &config);
        void CUIUI_EXPORT destroy();
        void CUIUI_EXPORT update();

        CUIUI_EXPORT void set_mouse_pos(uint32_t x, uint32_t y);

        static CUIUI_EXPORT LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
        static CUIUI_EXPORT void register_wc();
        static CUIUI_EXPORT void unregister_wc();

        constexpr operator HWND() {
            return hwnd;
        }
    };

    struct WindowBorderless;

    struct WindowBorderlessGlobals {
        WindowBorderless *grabbed_window = nullptr;
        WindowBorderless *hovered_window = nullptr;
        i32vec2 mouse_down_point = {};
        u32 grab_mode = 0;

        HCURSOR cursor_arrow = LoadCursorA(nullptr, IDC_ARROW);
        HCURSOR cursor_hand = LoadCursorA(nullptr, IDC_HAND);
        HCURSOR cursor_sizens = LoadCursorA(nullptr, IDC_SIZENS);
        HCURSOR cursor_sizewe = LoadCursorA(nullptr, IDC_SIZEWE);
        HCURSOR cursor_sizenwse = LoadCursorA(nullptr, IDC_SIZENWSE);
        HCURSOR cursor_sizenesw = LoadCursorA(nullptr, IDC_SIZENESW);
        HCURSOR cursor_current = cursor_arrow;
    };

    struct WindowBorderless : Window {
        using WindowHandleType = cuiui::WindowHandle<WindowBorderless>;

        i32vec2 pos;
        i32vec2 dim;
        i32vec2 min_dim{140, 140};
        bool maximized = false;

        static inline WindowBorderlessGlobals g = {};

        void CUIUI_EXPORT create(const cuiui::WindowConfig &config);
        void CUIUI_EXPORT update();

        static LRESULT CUIUI_EXPORT wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
        static void CUIUI_EXPORT register_wc();
        static void CUIUI_EXPORT unregister_wc();
    };

    template <typename WindowT>
    struct Context : public cuiui::Context<WindowT> {
        using WindowHandleType = cuiui::WindowHandle<WindowT>;
        static inline size_t ctx_count = 0;
        Context() {
            if (ctx_count > 0)
                return;
            ctx_count++;
            WindowT::register_wc();
        }
        ~Context() {
            ctx_count--;
            if (ctx_count > 0)
                return;
            WindowT::unregister_wc();
        }
        void update() {
            for (auto &[id, window] : cuiui::Context<WindowT>::windows)
                window->update();
        }
    };
} // namespace cuiui::platform::win32
#endif

#include <cuiui/platform/win32.hpp>
#include <utility>
#include <iostream>

namespace cuiui::platform::win32 {
    void CUIUI_EXPORT Window::create(const WindowConfig &config) {
        std::string title_str(config.title);
        hwnd = CreateWindowExA(
            0, window_class_name, title_str.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            static_cast<int32_t>(config.size.x), static_cast<int32_t>(config.size.y),
            nullptr, nullptr, GetModuleHandle(nullptr), this);
        ShowWindow(hwnd, SW_SHOW);
    }
    void CUIUI_EXPORT Window::destroy() {
        if (hwnd) {
            DestroyWindow(hwnd);
        }
    }
    void CUIUI_EXPORT Window::update() {
        MSG msg;
        while (PeekMessageW(&msg, hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    void Window::set_mouse_pos(uint32_t x, uint32_t y) {
        POINT pos = {static_cast<int32_t>(x), static_cast<int32_t>(y)};
        ClientToScreen(hwnd, &pos);
        SetCursorPos(pos.x, pos.y);
        mouse_pos = {static_cast<float>(x), static_cast<float>(y)};
    }

    LRESULT CUIUI_EXPORT Window::wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        if (msg == WM_NCCREATE) {
            const CREATESTRUCT &create_struct = *reinterpret_cast<const CREATESTRUCT *>(lp);
            if (create_struct.lpCreateParams)
                SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create_struct.lpCreateParams));
        }
        auto window_handle = reinterpret_cast<Window *>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
        if (window_handle) {
            auto &window = *window_handle;
            switch (msg) {
            case WM_CLOSE: {
                window.should_close = true;
            } break;
            case WM_SIZE: {
                window.size.x = lp & 0xffff;
                window.size.y = (lp & 0xffff0000) >> 16;
            } break;
            case WM_KEYDOWN: {
                window.events.push({
                    .type = EventType::KeyEvent,
                    .data = KeyEvent{
                        .key = uint32_t(wp),
                        .action = 1,
                    },
                });
            } break;
            case WM_KEYUP: {
                window.events.push({
                    .type = EventType::KeyEvent,
                    .data = KeyEvent{
                        .key = uint32_t(wp),
                        .action = 0,
                    },
                });
            } break;
            case WM_MOUSEMOVE: {
                auto new_pos = f32vec2{
                    static_cast<float>((lp & 0x0000ffff) >> 0x00),
                    static_cast<float>((lp & 0xffff0000) >> 0x10),
                };
                if (static_cast<int32_t>(window.mouse_pos.x) == static_cast<int32_t>(new_pos.x) &&
                    static_cast<int32_t>(window.mouse_pos.y) == static_cast<int32_t>(new_pos.y))
                    break;
                window.mouse_pos = new_pos;
                window.events.push({
                    .type = EventType::MouseMotionEvent,
                    .data = MouseMotionEvent{
                        .pos = new_pos,
                    },
                });
            } break;
            case WM_MOUSEWHEEL: {
                auto offset = f32vec2{
                    // static_cast<float>((lp & 0x0000ffff) >> 0x00),
                    // static_cast<float>((lp & 0xffff0000) >> 0x10),
                    0,
                    static_cast<float>(GET_WHEEL_DELTA_WPARAM(wp)),
                };
                window.events.push({
                    .type = EventType::MouseScrollEvent,
                    .data = MouseScrollEvent{
                        .offset = offset,
                    },
                });
            } break;
            case WM_LBUTTONDOWN: {
                window.events.push({.type = EventType::MouseButtonEvent, .data = MouseButtonEvent{.key = 0, .action = 1}});
            } break;
            case WM_LBUTTONUP: {
                window.events.push({.type = EventType::MouseButtonEvent, .data = MouseButtonEvent{.key = 0, .action = 0}});
            } break;
            case WM_RBUTTONDOWN: {
                window.events.push({.type = EventType::MouseButtonEvent, .data = MouseButtonEvent{.key = 1, .action = 1}});
            } break;
            case WM_RBUTTONUP: {
                window.events.push({.type = EventType::MouseButtonEvent, .data = MouseButtonEvent{.key = 1, .action = 0}});
            } break;
            case WM_MBUTTONDOWN: {
                window.events.push({.type = EventType::MouseButtonEvent, .data = MouseButtonEvent{.key = 2, .action = 1}});
            } break;
            case WM_MBUTTONUP: {
                window.events.push({.type = EventType::MouseButtonEvent, .data = MouseButtonEvent{.key = 2, .action = 0}});
            } break;
            default: {
                // std::array ignore{
                //     129, 131, 1, 24, 70, 70, 28, 134, 127, 127, 127,
                //     6, 641, 642, 7, 133, 20, 71, 3, 70, 36, 133, 20,
                //     71, 3, 129, 131, 1, 127, 136, 127, 127, 24, 70, 134,
                //     6, 70, 134, 127, 127, 127, 6, 8, 641, 641, 642, 642,
                //     7, 133, 20, 71, 3, 70, 36, 133, 20, 71, 3, 127, 799,
                //     127, 49375, 127, 36, 36, 799, 49375, 15, 15, 134, 6,
                //     28, 28, 8, 641, 642, 32, 132};
                // if (std::find(ignore.begin(), ignore.end(), msg) == ignore.end())
                //     std::cout << " " << msg << ",\n";
            } break;
            }
        }
        // return 0;
        return DefWindowProcA(hwnd, msg, wp, lp);
    };
    void CUIUI_EXPORT Window::register_wc() {
        WNDCLASSEXA window_class{
            .cbSize = sizeof(WNDCLASSEXA),
            .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
            .lpfnWndProc = wnd_proc,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = GetModuleHandle(nullptr),
            .hIcon = LoadIcon(NULL, IDI_APPLICATION),
            .hCursor = LoadCursor(NULL, IDC_ARROW),
            .hbrBackground = nullptr,
            .lpszMenuName = nullptr,
            .lpszClassName = window_class_name,
            .hIconSm = LoadIcon(NULL, IDI_WINLOGO),
        };
        RegisterClassExA(&window_class);
    }
    void CUIUI_EXPORT Window::unregister_wc() {
        UnregisterClassA(window_class_name, GetModuleHandleA(nullptr));
    }

    void CUIUI_EXPORT WindowBorderless::create(const cuiui::WindowConfig &config) {
        Window::create(config);

        RECT window_rect;
        GetWindowRect(hwnd, &window_rect);
        SetWindowPos(hwnd, NULL, window_rect.left, window_rect.top,
                     window_rect.right - window_rect.left,
                     window_rect.bottom - window_rect.top, SWP_FRAMECHANGED);
        pos = i32vec2{window_rect.top, window_rect.left};
        dim = i32vec2{window_rect.bottom, window_rect.right} - pos;
        DWMNCRENDERINGPOLICY policy = DWMNCRP_ENABLED;
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &policy, sizeof(policy));

        MARGINS margins{
            .cxLeftWidth = 0,
            .cxRightWidth = 0,
            .cyTopHeight = 1,
            .cyBottomHeight = 0,
        };

        DwmExtendFrameIntoClientArea(hwnd, &margins);
    }
    void CUIUI_EXPORT WindowBorderless::update() {
        Window::update();

        POINT mp;
        GetCursorPos(&mp);
        i32vec2 currentpos{mp.x, mp.y};
        i32rect window_rect{pos, pos + dim};

        auto hovered_edge = get_hovered_edge(window_rect, currentpos, 5);

        HCURSOR hovered_edge_cursor_table[]{
            g.cursor_arrow,
            g.cursor_sizewe,
            g.cursor_sizewe,
            nullptr,
            g.cursor_sizens,
            g.cursor_sizenwse,
            g.cursor_sizenesw,
            nullptr,
            g.cursor_sizens,
            g.cursor_sizenesw,
            g.cursor_sizenwse,
        };

        if (g.hovered_window == this && hovered_edge != 0xffffffff) {
            if (!g.grabbed_window)
                g.cursor_current = hovered_edge_cursor_table[hovered_edge];
            SetCursor(g.cursor_current);
        }

        if (g.grabbed_window == this) {
            if (maximized) {
                ShowWindow(hwnd, SW_NORMAL);
                auto old_pos = pos;
                RECT r;
                GetWindowRect(hwnd, &r);
                dim.x = r.right - r.left;
                dim.y = r.bottom - r.top;
                pos.x = r.left;
                pos.y = r.top;
                auto pos_diff = pos - old_pos;
                g.mouse_down_point -= pos_diff;
                maximized = false;
                window_rect = {pos, pos + dim};
            }
            drag(g.grab_mode, pos, dim, min_dim, currentpos, g.mouse_down_point);
            MoveWindow(hwnd, pos.x, pos.y, dim.x, dim.y, false);
            if (!GetAsyncKeyState(VK_LBUTTON)) {
                events.push({.type = EventType::MouseButtonEvent, .data = MouseButtonEvent{.key = 0, .action = 0}});
                g.grabbed_window = nullptr;
            }
        }
    }

    LRESULT CUIUI_EXPORT WindowBorderless::wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        if (msg == WM_NCCALCSIZE)
            if (wp)
                return 0;
        if (msg == WM_NCCREATE) {
            const CREATESTRUCT &create_struct = *reinterpret_cast<const CREATESTRUCT *>(lp);
            if (create_struct.lpCreateParams)
                SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create_struct.lpCreateParams));
        }
        auto window_handle = reinterpret_cast<WindowBorderless *>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
        if (g.grabbed_window && window_handle != g.grabbed_window)
            return -1;
        if (window_handle) {
            auto &window = *window_handle;
            switch (msg) {
            case WM_NCHITTEST: {
                if (g.grabbed_window)
                    return -1;
            } break;
            case WM_MOUSELAST: {
                g.hovered_window = nullptr;
            } break;
            case WM_LBUTTONDOWN: {
                g.grabbed_window = window_handle;
                POINT mp;
                GetCursorPos(&mp);
                g.mouse_down_point = i32vec2{mp.x, mp.y};
                RECT r;
                GetWindowRect(window.hwnd, &r);
                i32rect window_rect{{r.left, r.top}, {r.right, r.bottom}};
                window.pos = window_rect.p0;
                window.dim = window_rect.p1 - window_rect.p0;
                g.grab_mode = get_hovered_edge(window_rect, g.mouse_down_point, 5);
                g.mouse_down_point -= window_rect.p0;
                if (g.grab_mode & 0x02)
                    g.mouse_down_point.x = window.dim.x - g.mouse_down_point.x;
                if (g.grab_mode & 0x08)
                    g.mouse_down_point.y = window.dim.y - g.mouse_down_point.y;
            } break;
            case WM_LBUTTONUP: {
                g.grabbed_window = nullptr;
            } break;
            case WM_SYSCOMMAND: {
                switch (wp) {
                case SC_MAXIMIZE: {
                    window.maximized = true;
                } break;
                }
            } break;
            }
        }
        return Window::wnd_proc(hwnd, msg, wp, lp);
    }
    void CUIUI_EXPORT WindowBorderless::register_wc() {
        WNDCLASSEXA window_class{
            .cbSize = sizeof(WNDCLASSEXA),
            .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
            .lpfnWndProc = wnd_proc,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = GetModuleHandle(nullptr),
            .hIcon = LoadIcon(NULL, IDI_APPLICATION),
            .hCursor = LoadCursor(NULL, IDC_ARROW),
            .hbrBackground = nullptr,
            .lpszMenuName = nullptr,
            .lpszClassName = window_class_name,
            .hIconSm = LoadIcon(NULL, IDI_WINLOGO),
        };
        RegisterClassExA(&window_class);
    }
    void CUIUI_EXPORT WindowBorderless::unregister_wc() {
        UnregisterClassA(window_class_name, GetModuleHandleA(nullptr));
    }
} // namespace cuiui::platform::win32

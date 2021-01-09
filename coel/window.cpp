#include <coel/window.hpp>
#include <coel/application.hpp>

#include <Windows.h>
#include <exception>

namespace coel {
    static const char *window_class_name = "coel::window";
    static HINSTANCE instance_handle = GetModuleHandle(nullptr);

    struct win32_window_class {
        win32_window_class() {
            WNDCLASSEX w = {
                .cbSize = sizeof(WNDCLASSEX),
                .style = CS_HREDRAW | CS_VREDRAW,
                .lpfnWndProc = [](HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam) noexcept -> LRESULT {
                    auto app_ptr = reinterpret_cast<application *>(GetWindowLongPtr(window_handle, 0));
                    window *window_ptr = nullptr;

                    if (message_id == WM_NCCREATE) {
                        CREATESTRUCT *create_struct = (CREATESTRUCT *)lparam;
                        window_ptr = reinterpret_cast<window *>(create_struct->lpCreateParams);
                        SetWindowLongPtr(window_handle, GWLP_USERDATA, (LONG_PTR)window_ptr);
                    } else {
                        window_ptr = reinterpret_cast<window *>(GetWindowLongPtr(window_handle, GWLP_USERDATA));
                    }

                    if (message_id == WM_NCDESTROY) {
                        std::erase_if(app_ptr->windows, [&](const std::unique_ptr<window> &w) { return w->window_handle == window_handle; });
                        app_ptr->invalidated = true;
                    } else if (app_ptr && window_ptr) {
                        try {
                            window_ptr->on_event({
                                .parent_app = *app_ptr,
                                .parent_window = *window_ptr,
                                .native = {.message_id = message_id, .wparam = wparam, .lparam = lparam},
                            });
                        } catch (std::exception &e) {
                            MessageBox(nullptr, e.what(), "std::exception caught", MB_OK);
                            return -1;
                        } catch (...) {
                            MessageBox(nullptr, "caught an unknown exception inside message loop", "UNKNOWN EXCEPTION", MB_OK);
                            return -1;
                        }
                    }

                    return DefWindowProc(window_handle, message_id, wparam, lparam);
                },
                .cbClsExtra = 0,
                .cbWndExtra = sizeof(application *) + sizeof(window *),
                .hInstance = instance_handle,
                .hIcon = LoadIcon(instance_handle, IDI_APPLICATION),
                .hCursor = LoadCursor(instance_handle, IDC_ARROW),
                .lpszMenuName = nullptr,
                .lpszClassName = window_class_name,
                .hIconSm = LoadIcon(instance_handle, IDI_APPLICATION),
            };

            if (!RegisterClassEx(&w))
                throw std::exception("failed to register window class");
        }
    };

    window::window(const char *const title, const glm::ivec2 dim) {
        static win32_window_class window_class;

        window_handle = CreateWindow(
            window_class_name, title, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, dim.x, dim.y,
            nullptr, nullptr, instance_handle, this);

        ShowWindow(window_handle, SW_SHOW);
    }

    window::~window() {
    }
} // namespace coel

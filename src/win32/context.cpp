#include <cuiui/cuiui.hpp>
#include "types.hpp"

#include <utility>

namespace cuiui::platform {
    static void init_opengl(WindowHandle &window_handle) {
        auto &native = *window_handle->native_ptr;
        PIXELFORMATDESCRIPTOR pfd{
            .nSize = sizeof(PIXELFORMATDESCRIPTOR),
            .nVersion = 1,
            .dwFlags =
                PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            .iPixelType = PFD_TYPE_RGBA,
            .cColorBits = 32,
            .cAlphaBits = 8,
            .cDepthBits = 24,
            .cAuxBuffers = 0,
            .iLayerType = PFD_MAIN_PLANE,
        };
        native.hdc = GetDC(native.hwnd);
        auto choose_format = ChoosePixelFormat(native.hdc, &pfd);
        SetPixelFormat(native.hdc, choose_format, &pfd);
        native.hrc = wglCreateContext(native.hdc);
        wglMakeCurrent(native.hdc, native.hrc);
        UpdateWindow(native.hwnd);
    }

    static void deinit_opengl(WindowHandle &window_handle) {
        auto &native = *window_handle->native_ptr;
        if (native.hrc)
            wglDeleteContext(native.hrc);
        if (native.hdc && native.hwnd)
            ReleaseDC(native.hwnd, native.hdc);
    }

    void context_init() {
        WNDCLASSEXA window_class{
            .cbSize = sizeof(WNDCLASSEXA),
            .style = 0,
            .lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT {
                if (msg == WM_NCCREATE) {
                    const CREATESTRUCT &create_struct = *reinterpret_cast<const CREATESTRUCT *>(lp);
                    if (create_struct.lpCreateParams)
                        SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create_struct.lpCreateParams));
                }
                auto window_handle = reinterpret_cast<Window *>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
                if (window_handle) {
                    auto &window = *window_handle;
                    switch (msg) {
                    case WM_CLOSE:
                        window.should_close = true;
                        break;
                    case WM_SIZE:
                        window.size_x = lp & 0xffff;
                        window.size_y = (lp & 0xffff0000) >> 16;
                    }
                }
                return DefWindowProcA(hwnd, msg, wp, lp);
            },
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = GetModuleHandle(nullptr),
            .hIcon = nullptr,
            .hCursor = nullptr,
            .hbrBackground = nullptr,
            .lpszMenuName = nullptr,
            .lpszClassName = "cuiui_wc",
            .hIconSm = nullptr,
        };
        RegisterClassExA(&window_class);
    }

    void context_deinit() {
        UnregisterClassA("cuiui_wc", GetModuleHandleA(nullptr));
    }

    void context_update() {
    }

    void context_window_init(WindowHandle &window_handle, const WindowConfig &config) {
        window_handle->native_ptr = new WindowNativeData{};
        auto &native = *window_handle->native_ptr;
        native.hwnd = CreateWindowExA(
            0, "cuiui_wc", "DEFAULT TITLE", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            config.size_x, config.size_y,
            nullptr, nullptr, GetModuleHandle(nullptr), window_handle.get());
        ShowWindow(native.hwnd, SW_SHOW);
        init_opengl(window_handle);
    }

    void context_window_deinit(WindowHandle &window_handle) {
        deinit_opengl(window_handle);
        auto &native = *window_handle->native_ptr;
        if (native.hwnd)
            DestroyWindow(native.hwnd);
        delete window_handle->native_ptr;
    }

    void context_window_update(WindowHandle &window_handle) {
        MSG msg;
        while (PeekMessageW(&msg, window_handle->native_ptr->hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
} // namespace cuiui::platform

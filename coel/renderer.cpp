#include <coel/renderer.hpp>
#include <exception>

#include <gl/GL.h>
#include <gl/GLU.h>

namespace coel {
    renderer::renderer(const window &w) : window_handle(w.window_handle) {
        device_context_handle = GetDC(window_handle);

        PIXELFORMATDESCRIPTOR pixel_format_desc = {
            .nSize = sizeof(pixel_format_desc),
            .nVersion = 1,
            .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
            .iPixelType = PFD_TYPE_RGBA,
        };

        auto format_id = ChoosePixelFormat(device_context_handle, &pixel_format_desc);
        if (format_id == 0)
            throw std::exception("ChoosePixelFormat() failed: cannot find a suitable pixel format.");

        if (SetPixelFormat(device_context_handle, format_id, &pixel_format_desc) == FALSE) 
            throw std::exception("SetPixelFormat() failed: cannot set format specified");
        DescribePixelFormat(device_context_handle, format_id, sizeof(pixel_format_desc), &pixel_format_desc);

        opengl_rendering_context = wglCreateContext(device_context_handle);
        wglMakeCurrent(device_context_handle, opengl_rendering_context);

        RECT client_rect;
        GetClientRect(window_handle, &client_rect);

        frame_dim.x = client_rect.right - client_rect.left;
        frame_dim.y = client_rect.bottom - client_rect.top;

        wglMakeCurrent(nullptr, nullptr);
    }

    renderer::~renderer() {
        wglMakeCurrent(nullptr, nullptr);
        ReleaseDC(window_handle, device_context_handle);
        wglDeleteContext(opengl_rendering_context);
    }

    void renderer::on_event(const window_event &event) {
        if (event.native.message_id == WM_SIZE) {
            wglMakeCurrent(device_context_handle, opengl_rendering_context);
            frame_dim = {LOWORD(event.native.lparam), HIWORD(event.native.lparam)};
            glViewport(0, 0, frame_dim.x, frame_dim.y);
        } else if (event.native.message_id == WM_PAINT) {
            wglMakeCurrent(device_context_handle, opengl_rendering_context);
        }
    }
} // namespace coel

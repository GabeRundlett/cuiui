#pragma once
#include <Windows.h>
#include <glm/glm.hpp>

#include <coel/window.hpp>

namespace coel {
    class renderer {
        HDC device_context_handle;
        HGLRC opengl_rendering_context;

        HWND window_handle;

      public:
        glm::ivec2 frame_dim;

        renderer(const window &w);
        ~renderer();

        void on_event(const window_event &event);
    };
} // namespace coel

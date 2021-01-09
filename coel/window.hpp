#pragma once

#include <Windows.h>
#include <glm/glm.hpp>

namespace coel {
    class application;
    class window;

    struct window_event {
        application &parent_app;
        window &parent_window;

        struct native_window_event {
            UINT message_id;
            WPARAM wparam;
            LPARAM lparam;
        } native;
    };

    class window {
      public:
        HWND window_handle = nullptr;

        window(const char *const title, const glm::ivec2 dim);
        virtual ~window();

        friend application;
        virtual void on_event(const window_event &) {}
    };
} // namespace coel

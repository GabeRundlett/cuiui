#include <coel/application.hpp>

namespace coel {
    void application::init_window(const window &w) {
        SetWindowLongPtr(w.window_handle, 0, (LONG_PTR)this);
    }

    void application::update() {
        MSG message;

    app_update_beginning:
        invalidated = false;
        for (auto &w : windows) {
            while (PeekMessage(&message, w->window_handle, 0, 0, PM_REMOVE)) {
                try {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                } catch (...) {
                    invalidated = true;
                }
                if (invalidated)
                    goto app_update_beginning;
            }
        }

        while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }
} // namespace coel

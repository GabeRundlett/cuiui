#include <cuiui/cuiui.hpp>
#include "types.hpp"

namespace cuiui {
    void Window::bind_render_ctx() {
        wglMakeCurrent(native_ptr->hdc, native_ptr->hrc);
    }

    void Window::present() {
        SwapBuffers(native_ptr->hdc);
    }
}

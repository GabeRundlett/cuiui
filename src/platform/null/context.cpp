#include <cuiui/platform/null.hpp>

namespace cuiui::platform::null {
    void CUIUI_EXPORT Window::create(const WindowConfig &config) {
    }
    void CUIUI_EXPORT Window::destroy() {
    }
    void CUIUI_EXPORT Window::update() {
    }

    CUIUI_EXPORT Context::Context() {
    }
    CUIUI_EXPORT Context::~Context() {
    }
    void CUIUI_EXPORT Context::update() {
    }
} // namespace cuiui::platform::null

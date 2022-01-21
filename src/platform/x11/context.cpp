#include <cuiui/platform/x11.hpp>

namespace cuiui::platform::x11 {
    void CUIUI_EXPORT Window::create(const WindowConfig &config) {
        std::string title_str(config.title);
        display = XOpenDisplay(nullptr);
        if (display == nullptr)
            throw std::runtime_error("XLIB: Failed to open default display");
        auto screen = DefaultScreen(display);
        auto root = DefaultRootWindow(display);
        wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
        auto visual = (Visual *)CopyFromParent;
        auto depth = DefaultDepth(display, screen);
        XSetWindowAttributes wa = {0};
        wa.event_mask =
            StructureNotifyMask | KeyPressMask | KeyReleaseMask |
            PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
            ExposureMask | FocusChangeMask | VisibilityChangeMask |
            EnterWindowMask | LeaveWindowMask | PropertyChangeMask;
        xid = XCreateWindow(display, root, 0, 0, config.size_x, config.size_y, depth, CopyFromParent, InputOutput, visual, CWEventMask, &wa);
        XStoreName(display, xid, title_str.c_str());
        XSetWMProtocols(display, xid, &wm_delete_window, 1);
        XMapWindow(display, xid);
        auto _glfwPlatformGetTimerValue = [&]() -> uint64_t {
            struct timeval tv;
            gettimeofday(&tv, NULL);
            return (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)tv.tv_usec;
        };
        auto waitForEvent = [&](double *timeout) {
            fd_set fds;
            const int fd = ConnectionNumber(display);
            int count = fd + 1;
            for (;;) {
                FD_ZERO(&fds);
                FD_SET(fd, &fds);
                if (timeout) {
                    const long seconds = (long)*timeout;
                    const long microseconds = (long)((*timeout - seconds) * 1e6);
                    struct timeval tv = {seconds, microseconds};
                    const uint64_t base = _glfwPlatformGetTimerValue();
                    const int result = select(count, &fds, NULL, NULL, &tv);
                    const int error = errno;
                    *timeout -= (_glfwPlatformGetTimerValue() - base) / (double)1000000;
                    if (result > 0)
                        return true;
                    if ((result == -1 && error == EINTR) || *timeout <= 0.0)
                        return false;
                } else if (select(count, &fds, NULL, NULL, NULL) != -1 || errno != EINTR)
                    return true;
            }
        };
        auto waitForVisibilityNotify = [&](auto window) {
            XEvent dummy;
            double timeout = 0.1;
            while (!XCheckTypedWindowEvent(display, window, VisibilityNotify, &dummy)) {
                if (!waitForEvent(&timeout))
                    return false;
            }
            return true;
        };
        waitForVisibilityNotify(xid);
        XFlush(display);
    }
    void CUIUI_EXPORT Window::destroy() {
        XCloseDisplay(display);
    }
    void CUIUI_EXPORT Window::update() {
        XPending(display);
        while (XQLength(display)) {
            XEvent event;
            XNextEvent(display, &event);
            switch (event.type) {
            case ClientMessage:
                if (event.xclient.data.l[0] == wm_delete_window)
                    should_close = true;
                break;
            }
        }
        XFlush(display);
    }
} // namespace cuiui::platform::x11

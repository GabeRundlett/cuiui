#pragma once

#include <unordered_map>
#include <string_view>
#include <memory>
#include <queue>
#include <variant>

#include <cuiui/math/types.hpp>

namespace cuiui {
    struct KeyEvent {
        uint32_t key;
        uint32_t action;
    };
    struct MouseButtonEvent {
        uint32_t key;
        uint32_t action;
    };
    struct MouseMotionEvent {
        f32vec2 pos;
    };
    struct WindowMotionEvent {
        int32_t pos_x, pos_y;
    };
    struct WindowResizeEvent {
        int32_t size_x, size_y;
    };
    enum class EventType {
        KeyEvent,
        MouseButtonEvent,
        MouseMotionEvent,
        WindowMotionEvent,
        WindowResizeEvent,
    };

    struct Event {
        EventType type;
        std::variant<KeyEvent, MouseButtonEvent, MouseMotionEvent, WindowMotionEvent, WindowResizeEvent> data;
    };

    static constexpr auto x = sizeof(Event);

    struct WindowConfig {
        std::string_view id;
        std::string_view title;
        u32vec2 size;
    };

    struct WindowState {
        u32vec2 size;
        bool should_close;
        std::queue<Event> events;
    };

    template <typename WindowT>
    using WindowHandle = std::shared_ptr<WindowT>;

    template <typename WindowT>
    class Context {
      protected:
        using WindowHandleType = cuiui::WindowHandle<WindowT>;
        std::unordered_map<std::string_view, WindowHandleType> windows;

      public:
        Context() = default;

        ~Context() {
            for (auto &[key, w] : windows) {
                while (!w->events.empty())
                    w->events.pop();
                w->destroy();
            }
        }

        Context(const Context &) = delete;
        Context(Context &&) = delete;
        Context &operator=(const Context &) = delete;
        Context &operator=(Context &&) = delete;

        void update() {}

        WindowHandleType window(const WindowConfig &config) {
            auto find_window = windows.find(config.id);
            if (find_window != windows.end()) {
                auto &w = find_window->second;
                while (!w->events.empty())
                    w->events.pop();
                w->update();
                return w;
            }
            auto &w = (windows[config.id] = std::make_shared<WindowT>(WindowT{}));
            w->create(config);
            return w;
        }
    };
} // namespace cuiui

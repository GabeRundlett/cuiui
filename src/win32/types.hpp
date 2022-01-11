#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace cuiui {
    struct WindowNativeData {
        HWND hwnd;

        // OpenGL
        HDC hdc;
        HGLRC hrc;
    };
}

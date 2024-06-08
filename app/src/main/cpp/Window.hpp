#pragma once

#include <PlatformDefs.hpp>
#include <IOBuffer.hpp>

#if defined(T3D_WINDOWS)

#include <Windows.h>

#elif defined(T3D_ANDROID)

#include <android/native_window.h>
#include <android/input.h>
#include <android/rect.h>

#endif

class Window final {

public:

#if defined(T3D_WINDOWS)

    Window(HINSTANCE handle, int cmd_show, IOBuffer& io_buffer, const char* title, int2 position, int2 size);

#elif defined(T3D_ANDROID)

    Window(IOBuffer& io_buffer);

#else

    Window(IOBuffer& io_buffer);

#endif

    ~Window();

    void Update();
    bool IsOpen() const;

#if defined(T3D_DESKTOP)

    bool IsWindowed();
    bool IsFullscreen();

    [[nodiscard]] inline int2 GetSize() const {
        return m_size;
    }

    [[nodiscard]] inline dword GetRefreshRate() const {
        return m_refresh_rate;
    }

#endif

private:

#if defined(T3D_WINDOWS)

    HWND m_window;
    const char* m_title;
    int2 m_position;
    int2 m_size;
    dword m_refresh_rate;

#elif defined(T3D_ANDROID)

    ANativeWindow* m_window;
    ANativeWindow* m_pending_window;

    AInputQueue* m_input_queue;
    AInputQueue* m_pending_input_queue;

    ARect m_content_rect;
    ARect m_pending_content_rect;

#endif

    IOBuffer& m_io_buffer;
};
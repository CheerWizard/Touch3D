#pragma once

#include <PlatformDefs.hpp>
#include <IOBuffer.hpp>

#if defined(T3D_WINDOWS)

#include <Windows.h>

#elif defined(T3D_ANDROID)

#include <android/window.h>

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

#endif

    [[nodiscard]] inline int2 GetSize() const {
        return m_size;
    }

    [[nodiscard]] inline dword GetRefreshRate() const {
        return m_refresh_rate;
    }

private:
    void* m_handle;
    const char* m_title;
    int2 m_position;
    int2 m_size;
    dword m_refresh_rate;
    IOBuffer& m_io_buffer;
};
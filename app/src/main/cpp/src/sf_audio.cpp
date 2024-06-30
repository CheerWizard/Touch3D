#include <sf_audio.hpp>

namespace sf {

    void audio_init() {
        g_audio_system.devices = audio_device_get_all();
        SF_ASSERT(!g_audio_system.devices.empty(), "audio_system: no available devices in the system!");
        g_audio_system.selected_device = g_audio_system.devices[0];
    }

    void audio_free() {
    }

}

// SF_WINDOWS_BEGIN
#if defined(SF_WINDOWS)

namespace sf {
}

#endif
// SF_WINDOWS_END

// SF_LINUX_BEGIN
#if defined(SF_LINUX)

#include <pulse/pulseaudio.h>

namespace sf {

    vector_t<audio_device_t> audio_device_get_all() {
        vector_t<audio_device_t> devices;

        pa_threaded_mainloop* mainloop = pa_threaded_mainloop_new();

        pa_threaded_mainloop_start(mainloop);

        pa_threaded_mainloop_lock(mainloop);

        pa_mainloop_api* mainloop_api = pa_threaded_mainloop_get_api(mainloop);
        pa_context* context = pa_context_new_with_proplist(mainloop_api, "SF Audio", nullptr);

        void *udata = nullptr;

        pa_context_disconnect(context);

        pa_context_unref(context);

        pa_threaded_mainloop_unlock(mainloop);

        pa_threaded_mainloop_stop(mainloop);

        pa_threaded_mainloop_free(mainloop);

        return devices;
    }

    void audio_buffer_open(audio_buffer_t& audio_buffer, const audio_device_t& audio_device, const bool is_playback, u32 channels, u32 sample_rate, u32 buffer_length_usec) {
    }

    void audio_buffer_close(const audio_buffer_t& audio_buffer) {
    }

    void audio_buffer_play(audio_buffer_t &audio_buffer) {
    }

    void audio_buffer_record(audio_buffer_t &audio_buffer) {
    }

    void audio_buffer_drain(audio_buffer_t &audio_buffer) {
    }
}

#endif
// SF_LINUX_END

// SF_ANDROID_BEGIN
#if defined(SF_ANDROID)

namespace sf {
}

#endif
// SF_ANDROID_END
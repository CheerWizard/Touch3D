#include <sf_audio.hpp>

namespace sf {

    static audio_system_t s_audio_system = {};

    void audio_system_init() {
        s_audio_system.devices = audio_device_get_all();
        SF_ASSERT(!s_audio_system.devices.empty(), "audio_system: no available devices in the system!");
        s_audio_system.selected_device = s_audio_system.devices[0];
    }

    void audio_system_free() {
    }

}

// SF_WINDOWS_BEGIN
#if defined(SF_WINDOWS)

namespace sf {

    audio_loop_t audio_loop_init() {
        return {};
    }

    void audio_loop_free(const audio_loop_t &audio_loop) {

    }

    void audio_loop_start(audio_loop_t &audio_loop) {

    }

    void audio_loop_stop(audio_loop_t &audio_loop) {

    }

    vector_t<audio_device_t> audio_device_get_all() {
        return {};
    }

    void
    audio_buffer_open(audio_buffer_t &audio_buffer, const audio_device_t &audio_device, bool is_playback, u32 channels,
                      u32 sample_rate, u32 buffer_length_usec) {

    }

    void audio_buffer_close(const audio_buffer_t &audio_buffer) {

    }

    void audio_buffer_play(audio_buffer_t &audio_buffer) {

    }

    void audio_buffer_record(audio_buffer_t &audio_buffer) {

    }

    void audio_buffer_drain(audio_buffer_t &audio_buffer) {

    }

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
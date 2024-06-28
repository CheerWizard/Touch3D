#pragma once

#include <sf.hpp>

#define SF_AUDIO_SAMPLE_TO_DB(s) log10((double)(s) * (1 / 32768.0)) * 20

#define SF_AUDIO_CHANNELS 2
#define SF_AUDIO_SAMLE_RATE 48000
#define SF_AUDIO_BUFFER_LENGTH_USEC 500 * 1000

enum SF_AUDIO_DEVICE_TYPE {
    SF_AUDIO_DEVICE_TYPE_PLAYBACK,
    SF_AUDIO_DEVICE_TYPE_RECORDER,
};

namespace sf {

    struct SF_API audio_device_t final {
        string_t id;
        string_t name;
        SF_AUDIO_DEVICE_TYPE type;
    };

    SF_API vector_t<audio_device_t> audio_device_get_all();

    struct SF_API audio_buffer_t final {
        void* handle = nullptr;
        u32 frame_size = 0;
        u32 buffer_size = 0;
    };

    SF_API void audio_buffer_open(
        audio_buffer_t& audio_buffer,
        const audio_device_t& audio_device,
        bool is_playback,
        u32 channels = SF_AUDIO_CHANNELS,
        u32 sample_rate = SF_AUDIO_SAMLE_RATE,
        u32 buffer_length_usec = SF_AUDIO_BUFFER_LENGTH_USEC
    );
    SF_API void audio_buffer_close(const audio_buffer_t& audio_buffer);
    SF_API void audio_buffer_play(audio_buffer_t& audio_buffer);
    SF_API void audio_buffer_record(audio_buffer_t& audio_buffer);
    SF_API void audio_buffer_drain(audio_buffer_t& audio_buffer);

    struct SF_API audio_system_t final {
        vector_t<audio_device_t> devices;
        audio_device_t selected_device;
    };

    extern audio_system_t global_audio_system;

    SF_API void audio_system_init();
    SF_API void audio_system_free();

}
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

    struct SF_API sound_t final {
        usize size = 0;
        void* data = nullptr;
    };

    struct SF_API sound_player_t final {
        vector_t<sound_t> sounds;
    };

    SF_API sound_player_t sound_player_init();
    SF_API void sound_player_free(const sound_player_t& sound_player);
    SF_API void sound_player_start(sound_player_t& sound_player);
    SF_API void sound_player_stop(sound_player_t& sound_player);
    SF_API void sound_player_add_sound(sound_player_t& sound_player, const sound_t& sound);

    struct SF_API sound_recorder_t final {};

    SF_API sound_recorder_t sound_recorder_init();
    SF_API void sound_recorder_free(const sound_recorder_t& sound_recorder);
    SF_API void sound_recorder_start(sound_recorder_t& sound_recorder);
    SF_API void sound_recorder_stop(sound_recorder_t& sound_recorder);

    SF_API void audio_play(const audio_t& audio);
    SF_API void audio_play_from_file(const char* filepath);
    SF_API void audio_write_to_file(const char* filepath, char*);

    struct SF_API audio_allocator_t final {
        void* allocate(usize size, usize alignment = SF_ALIGNMENT);
        void deallocate(void* addr);
    };

    struct SF_API audio_loop_t final {
        void* handle;
        bool running = false;
        thread_t thread;
        circular_buffer_t<task_t, audio_allocator_t> task_buffer;
    };

    SF_API audio_loop_t audio_loop_init();
    SF_API void audio_loop_free(const audio_loop_t& audio_loop);
    SF_API void audio_loop_start(audio_loop_t& audio_loop);
    SF_API void audio_loop_stop(audio_loop_t& audio_loop);

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

    SF_API void audio_system_init();
    SF_API void audio_system_free();

}
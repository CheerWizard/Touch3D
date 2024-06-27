#pragma once

#include <sf.hpp>

#define SF_AUDIO_SAMPLE_TO_DB(s) log10((double)(s) * (1 / 32768.0)) * 20

namespace sf {

    struct SF_API audio_device_t final {
        void* handle;
    };

    struct SF_API audio_card_t final {
        void* handle;
        vector_t<audio_device_t> devices;
    };

    struct SF_API audio_t final {
        vector_t<audio_card_t> cards;
        audio_card_t card_selected;
        audio_device_t device_selected;
    };

    extern audio_t global_audio;

    SF_API void audio_init();

}
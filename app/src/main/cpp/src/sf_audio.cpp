#include <sf_audio.hpp>

namespace sf {

    void audio_init() {
        global_audio_system.devices = audio_device_get_all();
        SF_ASSERT(!global_audio_system.devices.empty(), "audio_system: no available devices in the system!");
        global_audio_system.selected_device = global_audio_system.devices[0];
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

#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>

namespace sf {

    vector_t<audio_device_t> audio_device_get_all() {
        vector_t<audio_device_t> devices;

        int mixer = open("/dev/mixer", O_RDONLY, 0);
        oss_sysinfo si = {};
        ioctl(mixer, SNDCTL_SYSINFO, &si);
        int n_devs = si.numaudios;
        close(mixer);

        return devices;
    }

    void audio_buffer_open(audio_buffer_t& audio_buffer, const audio_device_t& audio_device, const bool is_playback, u32 channels, u32 sample_rate, u32 buffer_length_usec) {
        snd_pcm_t* snd_pcm = nullptr;
        snd_pcm_open(&snd_pcm, audio_device.id.data(), snd_pcm_stream(snd_pcm), is_playback ? SND_PCM_STREAM_PLAYBACK : SND_PCM_STREAM_CAPTURE);
        audio_buffer.handle = snd_pcm;

        snd_pcm_hw_params_t *params;
        snd_pcm_hw_params_alloca(&params);
        snd_pcm_hw_params_any(snd_pcm, params);

        snd_pcm_hw_params_set_access(snd_pcm, params, SND_PCM_ACCESS_MMAP_INTERLEAVED);
        snd_pcm_hw_params_set_format(snd_pcm, params, SND_PCM_FORMAT_S16_LE);
        snd_pcm_hw_params_set_channels_near(snd_pcm, params, &channels);
        snd_pcm_hw_params_set_rate_near(snd_pcm, params, &sample_rate, nullptr);
        snd_pcm_hw_params_set_buffer_time_near(snd_pcm, params, &buffer_length_usec, nullptr);
        audio_buffer.frame_size = (16/8) * channels;
        audio_buffer.buffer_size = sample_rate * (16/8) * channels * buffer_length_usec / 1000000;

        snd_pcm_hw_params(snd_pcm, params);
    }

    void audio_buffer_close(const audio_buffer_t& audio_buffer) {
        snd_pcm_close(static_cast<snd_pcm_t*>(audio_buffer.handle));
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
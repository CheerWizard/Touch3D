#include <sf_audio.hpp>

namespace sf {



}

// SF_WINDOWS_BEGIN
#if defined(SF_WINDOWS)

namespace sf {
}

#endif
// SF_WINDOWS_END

// SF_LINUX_BEGIN
#if defined(SF_LINUX)

#include <alsa/asoundlib.h>

namespace sf {

    void audio_get_cards() {

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
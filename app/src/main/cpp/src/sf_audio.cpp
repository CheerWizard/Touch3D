#include <sf_audio.hpp>

namespace sf {

}

// SF_WINDOWS_BEGIN
#if defined(SF_WINDOWS)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xaudio2.h>

IXAudio2* xaudio;
IXAudio2MasteringVoice* xaudioMasterVoice;
IXAudio2SourceVoice* xaudioSourceVoice;
XAUDIO2_BUFFER xaudioBuffer;
bool audioBusy;

////////////////////////////////////////////////////////////////////////////////////
// The MIT License (MIT)
//
// Copyright (c) 2021 Tarek Sherif
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xaudio2.h>
#include <stdbool.h>

IXAudio2* xaudio;
IXAudio2MasteringVoice* xaudioMasterVoice;
IXAudio2SourceVoice* xaudioSourceVoice;
XAUDIO2_BUFFER xaudioBuffer;
bool audioBusy;

///////////////////////////////////////////////////////////
// Set up callbacks for our source voice to track when it
// finishes playing. We need to define all the callbacks,
// so the rest are just stubs.
///////////////////////////////////////////////////////////

void OnBufferEnd(IXAudio2VoiceCallback* This, void* pBufferContext)    {
    audioBusy = false;
}

void OnStreamEnd(IXAudio2VoiceCallback* This) { }
void OnVoiceProcessingPassEnd(IXAudio2VoiceCallback* This) { }
void OnVoiceProcessingPassStart(IXAudio2VoiceCallback* This, UINT32 SamplesRequired) { }
void OnBufferStart(IXAudio2VoiceCallback* This, void* pBufferContext) { }
void OnLoopEnd(IXAudio2VoiceCallback* This, void* pBufferContext) { }
void OnVoiceError(IXAudio2VoiceCallback* This, void* pBufferContext, HRESULT Error) { }

///////////////////////////////////////////////////////////////
// The trick to setting up callbacks in C is that the function
// pointers go in the 'lpVtbl' property, which is of type
// IXAudio2VoiceCallbackVtbl*. (In C++, this is done by
// inheriting from IXAudio2VoiceCallback.)
///////////////////////////////////////////////////////////
IXAudio2VoiceCallback xAudioCallbacks = {
        .lpVtbl = &(IXAudio2VoiceCallbackVtbl) {
                .OnStreamEnd = OnStreamEnd,
                .OnVoiceProcessingPassEnd = OnVoiceProcessingPassEnd,
                .OnVoiceProcessingPassStart = OnVoiceProcessingPassStart,
                .OnBufferEnd = OnBufferEnd,
                .OnBufferStart = OnBufferStart,
                .OnLoopEnd = OnLoopEnd,
                .OnVoiceError = OnVoiceError
        }
};

typedef struct {
    BYTE* data;
    UINT32 size;
    WAVEFORMATEXTENSIBLE format;
} AudioData;

AudioData loadAudioData(void);

///////////////////////////////////////////////////////////
// Play a sound by submitting an XAUDIO_BUFFER to the
// source voice.
///////////////////////////////////////////////////////////
void playSound() {
    if (!audioBusy) {
        ///////////////////////////////////////////////////////////
        // This is the general pattern when using XAudio2 (or
        // anything COM) in C. These are macros that have names
        // similar to the C++ names seen on MSDN, e.g.
        // IXAudio2SourceVoice::SubmitSourceBuffer becomes
        // IXAudio2SourceVoice_SubmitSourceBuffer and takes the
        // IXAudio2SourceVoice pointer as its first argument.
        ///////////////////////////////////////////////////////////
        IXAudio2SourceVoice_SubmitSourceBuffer(xaudioSourceVoice, &xaudioBuffer, NULL);
        audioBusy = true;
    }
}

LRESULT CALLBACK winProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_LBUTTONDOWN:
        case WM_KEYDOWN: {
            playSound();
            return 0;
        } break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(window, &ps);
            char text[] = "Press any key or click mouse to play a sound!";
            TextOut(hdc, 20, 20, text, sizeof(text) - 1);
            EndPaint(window, &ps);
            return 0;
        } break;
        case WM_CLOSE: {
            PostQuitMessage(0);
            return 0;
        } break;
    }

    return DefWindowProc(window, message, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int showWindow) {
    ///////////////////////////////////////////////////////////
    // Set up window
    ///////////////////////////////////////////////////////////

    const char WIN_CLASS_NAME[] = "XAUDIO2_DEMO_WINDOW_CLASS";

    WNDCLASSEX winClass = {
            .cbSize = sizeof(winClass),
            .style = CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc = winProc,
            .hInstance = instance,
            .hIcon = LoadIcon(instance, IDI_APPLICATION),
            .hIconSm = LoadIcon(instance, IDI_APPLICATION),
            .hCursor = LoadCursor(NULL, IDC_ARROW),
            .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
            .lpszClassName = WIN_CLASS_NAME
    };

    if (!RegisterClassEx(&winClass)) {
        MessageBox(NULL, "Failed to register window class!", "FAILURE", MB_OK);

        return 1;
    }

    HWND window = CreateWindow(
            WIN_CLASS_NAME,
            "XAudio2 C Demo",
            WS_OVERLAPPEDWINDOW,
            0,
            0,
            400,
            400,
            NULL,
            NULL,
            instance,
            NULL
    );

    if (!window) {
        MessageBox(NULL, "Failed to create window!", "FAILURE", MB_OK);
        return 1;
    }

    ///////////////////////////////////////////////////////////
    // Load audio data from sample wave file.
    ///////////////////////////////////////////////////////////
    AudioData audioData = loadAudioData();

    if (!audioData.data) {
        MessageBox(NULL, "Failed to load audio data!", "FAILURE", MB_OK);
        return 1;
    }
}

AudioData loadAudioData(void) {
    AudioData result = { 0 };

    HANDLE audioFile = CreateFileA(
            "sound.wav",
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
    );

    if (audioFile == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, "Failed to load audio!", "FAILURE", MB_OK);
        return result;
    }

    if (SetFilePointer(audioFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
        MessageBox(NULL, "Failed to set file pointer!", "FAILURE", MB_OK);
        return result;
    }

    DWORD chunkType;
    DWORD chunkDataSize;
    DWORD fileFormat;
    DWORD bytesRead = 0;

    ReadFile(audioFile, &chunkType, sizeof(DWORD), &bytesRead, NULL);     // RIFF chunk

    if (chunkType != 'FFIR') {
        CloseHandle(audioFile);
        return result;
    }

    ReadFile(audioFile, &chunkDataSize, sizeof(DWORD), &bytesRead, NULL); // Data size (for all subchunks)
    ReadFile(audioFile, &fileFormat, sizeof(DWORD), &bytesRead, NULL);    // WAVE format

    if (fileFormat != 'EVAW') {
        CloseHandle(audioFile);
        return result;
    }

    ReadFile(audioFile, &chunkType, sizeof(DWORD), &bytesRead, NULL);     // First subchunk (should be 'fmt')

    if (chunkType != ' tmf') {
        CloseHandle(audioFile);
        return result;
    }

    ReadFile(audioFile, &chunkDataSize, sizeof(DWORD), &bytesRead, NULL); // Data size for format
    ReadFile(audioFile, &result.format, chunkDataSize, &bytesRead, NULL); // Wave format struct

    ReadFile(audioFile, &chunkType, sizeof(DWORD), &bytesRead, NULL);     // Next subchunk (should be 'data')

    if (chunkType != 'atad') {
        CloseHandle(audioFile);
        return result;
    }

    ReadFile(audioFile, &chunkDataSize, sizeof(DWORD), &bytesRead, NULL); // Data size for data

    BYTE* audioData = (BYTE*) malloc(chunkDataSize);

    if (!audioData) {
        CloseHandle(audioFile);
        return result;
    }

    ReadFile(audioFile, audioData, chunkDataSize, &bytesRead, NULL);      // FINALLY!

    result.size = chunkDataSize;
    result.data = audioData;

    CloseHandle(audioFile);

    return result;
}

namespace sf {

    static void audio_init() {
        if (FAILED(XAudio2Create(&xaudio, 0, XAUDIO2_DEFAULT_PROCESSOR ))) {
            MessageBox(NULL, "Failed to initialize XAudio!", "FAILURE", MB_OK);
            SF_ASSERT(false, "Failed to initialize XAudio!");
            return;
        }
        if (FAILED(IXAudio2_CreateMasteringVoice(
                xaudio,
                &xaudioMasterVoice,
                XAUDIO2_DEFAULT_CHANNELS,
                XAUDIO2_DEFAULT_SAMPLERATE,
                0,
                NULL,
                NULL,
                AudioCategory_GameEffects
        ))) {
            MessageBox(NULL, "Failed to initialize XAudio mastering voice!", "FAILURE", MB_OK);
            SF_ASSERT(false, "Failed to initialize XAudio mastering voice!");
            return;
        }
        if (FAILED(IXAudio2_CreateSourceVoice(
                xaudio,
                &xaudioSourceVoice,
                &audioData.format.Format,
                0,
                XAUDIO2_DEFAULT_FREQ_RATIO,
                &xAudioCallbacks,
                NULL,
                NULL
        ))) {
            MessageBox(NULL, "Failed to initialize XAudio source voice!", "FAILURE", MB_OK);
            SF_ASSERT(false, "Failed to initialize XAudio source voice!");
            return;
        }
        IXAudio2SourceVoice_Start(xaudioSourceVoice, 0, XAUDIO2_COMMIT_NOW);
        xaudioBuffer.AudioBytes = audioData.size;
        xaudioBuffer.pAudioData = audioData.data;
        xaudioBuffer.Flags = XAUDIO2_END_OF_STREAM;
    }

    static void audio_free() {

    }

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
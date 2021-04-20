#include "LibretroMoonlightSessionDecoderAndRenderProvider.hpp"
#include "FFmpegVideoDecoder.hpp"
#include "GLVideoRenderer.hpp"
#include "DebugFileRecorderAudioRenderer.hpp"

IFFmpegVideoDecoder* LibretroMoonlightSessionDecoderAndRenderProvider::video_decoder() {
    return new FFmpegVideoDecoder();
}

IVideoRenderer* LibretroMoonlightSessionDecoderAndRenderProvider::video_renderer() {
    return new GLVideoRenderer();
}

IAudioRenderer* LibretroMoonlightSessionDecoderAndRenderProvider::audio_renderer() {
    return new DebugFileRecorderAudioRenderer(false);
}

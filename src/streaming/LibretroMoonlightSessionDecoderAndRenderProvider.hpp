#include "MoonlightSessionDecoderAndRenderProvider.hpp"

class LibretroMoonlightSessionDecoderAndRenderProvider: public MoonlightSessionDecoderAndRenderProvider {
public:
    LibretroMoonlightSessionDecoderAndRenderProvider() {};
    
    IFFmpegVideoDecoder* video_decoder();
    IVideoRenderer* video_renderer();
    IAudioRenderer* audio_renderer();
};

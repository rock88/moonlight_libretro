#include <stdio.h>
#include <curl/curl.h>
#include <openssl/ssl.h>
#include "glsym/glsym.h"
#include "libretro.h"
#include "Application.hpp"
#include "Settings.hpp"
#include "GameStreamClient.hpp"
#include "MouseController.hpp"
#include "KeyboardController.hpp"
#include "GamepadController.hpp"
#include "LibretroMouseFrontend.hpp"
#include "DummyKeyboardFrontend.hpp"
#include "DummyGamepadFrontend.hpp"
#include "MoonlightSession.hpp"
#include "LibretroMoonlightSessionDecoderAndRenderProvider.hpp"
#include "Logger.hpp"

#if defined(HAVE_PSGL)
#define RARCH_GL_FRAMEBUFFER GL_FRAMEBUFFER_OES
#define RARCH_GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_OES
#define RARCH_GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_EXT
#elif defined(OSX_PPC)
#define RARCH_GL_FRAMEBUFFER GL_FRAMEBUFFER_EXT
#define RARCH_GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_EXT
#define RARCH_GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_EXT
#else
#define RARCH_GL_FRAMEBUFFER GL_FRAMEBUFFER
#define RARCH_GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE
#define RARCH_GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0
#endif

#if OSX
extern "C" unsigned int macos_get_backing_scale_factor();
#endif

unsigned int width  = 1280;
unsigned int height = 720;
unsigned int framebuffer_width = width;
unsigned int framebuffer_height = height;

retro_hw_render_callback hw_render;
retro_video_refresh_t video_cb;
retro_audio_sample_t audio_cb;
retro_audio_sample_batch_t audio_batch_cb;
retro_environment_t environment_cb;
retro_input_poll_t input_poll_cb;
retro_input_state_t input_state_cb;

static Application* app = NULL;
int moonlight_exit = 0;

void retro_init(void) {
    OpenSSL_add_all_algorithms();
    //curl_global_init(CURL_GLOBAL_ALL);
    
#if OSX
    framebuffer_width = width * macos_get_backing_scale_factor();
    framebuffer_height = height * macos_get_backing_scale_factor();
#endif
    
    GameStreamClient::instance().start();
    
    MoonlightSession::set_provider(new LibretroMoonlightSessionDecoderAndRenderProvider());
    MouseController::instance().init(new LibretroMouseFrontend());
    KeyboardController::instance().init(new DummyKeyboardFrontend());
    GamepadController::instance().init(new DummyGamepadFrontend());
}

void retro_deinit(void) {
    GameStreamClient::instance().stop();
    Logger::info("moonlight", "retro_deinit");
}

static void context_reset(void) {
    fprintf(stderr, "Context reset!\n");
    rglgen_resolve_symbols(hw_render.get_proc_address);
    
    nanogui::init();
    nanogui::setup(1.0 / 15.0);
    
    if (app) {
        delete app;
    }
    
    app = new Application({ (float)width, (float)height }, { (float)framebuffer_width, (float)framebuffer_height });
}

static void context_destroy(void) {
    fprintf(stderr, "Context destroy!\n");
    
    //delete app;
    nanogui::leave();
}

void retro_run(void) {
    input_poll_cb();
    
    MouseController::instance().handle_mouse();
    KeyboardController::instance().handle_keyboard();
    GamepadController::instance().handle_gamepad();
    
    glBindFramebuffer(RARCH_GL_FRAMEBUFFER, hw_render.get_current_framebuffer());
    
    glViewport(0, 0, framebuffer_width, framebuffer_height);
    
    nanogui::draw();
    MouseController::instance().draw_cursor(app);
    
    video_cb(RETRO_HW_FRAME_BUFFER_VALID, framebuffer_width, framebuffer_height, 0);
}

static bool retro_init_hw_context(void) {
#if defined(GL_CORE)
    hw_render.context_type = RETRO_HW_CONTEXT_OPENGL_CORE;
    hw_render.version_major = 3;
    hw_render.version_minor = 2;
#else
    hw_render.context_type = RETRO_HW_CONTEXT_OPENGL;
#endif
    hw_render.context_reset = context_reset;
    hw_render.context_destroy = context_destroy;
    hw_render.depth = true;
    hw_render.stencil = true;
    hw_render.bottom_left_origin = true;
    
    if (!environment_cb(RETRO_ENVIRONMENT_SET_HW_RENDER, &hw_render)) {
        return false;
    }
    return true;
}

unsigned retro_api_version(void) {
    return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device) {
    (void)port;
    (void)device;
}

void retro_get_system_info(struct retro_system_info *info) {
    memset(info, 0, sizeof(*info));
    info->library_name     = "Moonlight";
    info->library_version  = "v" MOONLIGHT_VERSION;
    info->need_fullpath    = false;
    info->valid_extensions = NULL; // Anything is fine, we don't care.
}

void retro_get_system_av_info(struct retro_system_av_info *info) {
    info->timing = (struct retro_system_timing) {
        .fps = 60.0,
        .sample_rate = 48000,
    };
    
    info->geometry = (struct retro_game_geometry) {
        .base_width   = width,
        .base_height  = height,
        .max_width    = framebuffer_width,
        .max_height   = framebuffer_height,
        .aspect_ratio = 16.0 / 9.0,
    };
}

void retro_set_environment(retro_environment_t cb) {
    environment_cb = cb;
    
    bool no_game = true;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_game);
    
    const char *dir = NULL;
    cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir);
    
    if (dir != NULL) {
        fprintf(stderr, "dir: %s\n", dir);
        Settings::instance().set_working_dir(std::string(dir) + "/moonlight");
    }
}

void retro_set_audio_sample(retro_audio_sample_t cb) {
    audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) {
    audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb) {
    input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb) {
    input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb) {
    video_cb = cb;
}

bool retro_load_game(const struct retro_game_info *info) {
    enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
    if (!environment_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt)) {
        fprintf(stderr, "XRGB8888 is not supported.\n");
        return false;
    }
    
    if (!retro_init_hw_context()) {
        fprintf(stderr, "HW Context could not be initialized, exiting...\n");
        return false;
    }
    
    fprintf(stderr, "Loaded game!\n");
    (void)info;
    return true;
}

void retro_unload_game(void) {
    
}

unsigned retro_get_region(void) {
    return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num) {
    (void)type;
    (void)info;
    (void)num;
    return false;
}

size_t retro_serialize_size(void) {
    return 0;
}

bool retro_serialize(void *data, size_t size) {
    (void)data;
    (void)size;
    return false;
}

bool retro_unserialize(const void *data, size_t size) {
    (void)data;
    (void)size;
    return false;
}

void *retro_get_memory_data(unsigned id) {
    (void)id;
    return NULL;
}

size_t retro_get_memory_size(unsigned id) {
    (void)id;
    return 0;
}

void retro_reset(void){
    
}

void retro_cheat_reset(void){
    
}

void retro_cheat_set(unsigned index, bool enabled, const char *code) {
    (void)index;
    (void)enabled;
    (void)code;
}

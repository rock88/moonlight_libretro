#include "LibretroMouseFrontend.hpp"
#include "libretro.h"
#include "Logger.hpp"

extern retro_input_state_t input_state_cb;

#if defined(OSX)
extern "C" void macos_get_mouse_position(int* x, int* y);
#elif defined(_WIN32)
extern "C" void windows_get_mouse_position(int* x, int* y);
#endif

LibretroMouseFrontend::LibretroMouseFrontend() {
    
}

void LibretroMouseFrontend::handle_mouse() {
    m_mouse_state.l_pressed = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT);
    m_mouse_state.m_pressed = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_MIDDLE);
    m_mouse_state.r_pressed = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT);
    
#if defined(OSX)
    macos_get_mouse_position(&m_mouse_state.x, &m_mouse_state.y);
#elif defined(_WIN32)
    windows_get_mouse_position(&m_mouse_state.x, &m_mouse_state.y);
#else
    int mouse_x = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
    int mouse_y = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);
    
    m_mouse_state.x += mouse_x;
    m_mouse_state.y += mouse_y;
#endif
}

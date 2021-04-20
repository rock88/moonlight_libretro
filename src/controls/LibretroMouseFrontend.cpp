#include "LibretroMouseFrontend.hpp"
#include "libretro.h"
#include "Logger.hpp"

extern retro_input_state_t input_state_cb;

LibretroMouseFrontend::LibretroMouseFrontend() {
    
}

void LibretroMouseFrontend::handle_mouse() {
    m_mouse_state.l_pressed = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT);
    m_mouse_state.m_pressed = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_MIDDLE);
    m_mouse_state.r_pressed = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT);
    
    int mouse_x = input_state_cb(1, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
    int mouse_y = input_state_cb(1, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);
    
    m_mouse_state.x += mouse_x;
    m_mouse_state.y += mouse_y;
    //Logger::info("mouse", "%ix%i", m_mouse_state.x, m_mouse_state.y);
    
    int pointer_x = 0, pointer_y = 0;
    
    // TODO: Pointer on macOS currently broken?
    // https://github.com/libretro/RetroArch/issues/10168
    //if (input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_PRESSED)) {
    int width = 1280;
    int height = 720;
    
    const float X_FACTOR = ((float)width / 65536.0f);
    const float Y_FACTOR = ((float)height / 65536.0f);
    
    float x = (input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X) + 32768.0f) * X_FACTOR;
    float y = (input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y) + 32768.0f) * Y_FACTOR;
    
    pointer_x = x;//(input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X) + 0x7fff) * width / 0xffff;
    pointer_y = y;//(input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y) + 0x7fff) * height / 0xffff;
    
//    m_mouse_state.x = pointer_x;// / 0x10000;
//    m_mouse_state.y = pointer_y;// / 0x10000;
    //m_mouse_state.x += mouse_x;
   // m_mouse_state.y += mouse_y;
    
    //Logger::info("mouse", "%ix%i", m_mouse_state.x, m_mouse_state.y);
    
        //Logger::info("pointer", "%ix%i", pointer_x, pointer_y);
    //}
}

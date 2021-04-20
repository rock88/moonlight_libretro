#include "MouseFrontend.hpp"
#pragma once

class LibretroMouseFrontend: public MouseFrontend {
public:
    LibretroMouseFrontend();
    
    void handle_mouse();
    
    MouseState mouse_state() const {
        return m_mouse_state;
    }
    
    bool hid_mouse_is_used() const {
        return true;
    }
    
private:
    MouseState m_mouse_state = {0};
};

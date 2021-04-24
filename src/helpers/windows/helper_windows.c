#include <stdio.h>
#include <windows.h>
#include <winuser.h>

extern unsigned int width;
extern unsigned int height;

void windows_get_mouse_position(int* x, int* y) {
    HWND hwnd = GetActiveWindow();
    
    if (hwnd == NULL) {
        return;
    }
    
    POINT point;
    
    if (!GetCursorPos(&point)) {
        return;
    }
    
    if (!ScreenToClient(hwnd, &point)) {
        return;
    }
    
    RECT rect;
    
    if (!GetClientRect(hwnd, &rect)) {
        return;
    }
    
    float rect_width = rect.right - rect.left;
    float rect_height = rect.bottom - rect.top;
    
    float w = width, h = height, w_r = 0, h_r = 0;
    float aspect = w / h;
    
    if (rect_width / aspect <= rect_height) {
        w_r = rect_width;
        h_r = rect_width / aspect;
    } else {
        w_r = rect_height * aspect;
        h_r = rect_height;
    }
    
    float px = point.x - (rect_width - w_r) / 2;
    float py = point.y - (rect_height - h_r) / 2;
    
    *x = px * w / w_r;
    *y = py * h / h_r;
}

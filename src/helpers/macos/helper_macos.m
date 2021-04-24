#include <AppKit/AppKit.h>

extern unsigned int width;
extern unsigned int height;

unsigned int macos_get_backing_scale_factor() {
    return [NSScreen mainScreen].backingScaleFactor;
}

void macos_get_mouse_position(int* x, int* y) {
    NSWindow* window = [[[NSApplication sharedApplication] windows] firstObject];
    NSPoint point = window.mouseLocationOutsideOfEventStream;
    NSRect rect = window.contentView.frame;
    
    float w = width, h = height, w_r = 0, h_r = 0;
    float aspect = w / h;
    
    if (rect.size.width / aspect <= rect.size.height) {
        w_r = rect.size.width;
        h_r = rect.size.width / aspect;
    } else {
        w_r = rect.size.height * aspect;
        h_r = rect.size.height;
    }
    
    float px = point.x - (rect.size.width - w_r) / 2;
    float py = rect.size.height - point.y - (rect.size.height - h_r) / 2;
    
    *x = px * w / w_r;
    *y = py * h / h_r;
}

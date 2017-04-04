#include <kgraphics.h>

struct kgraphics kgraphics;

void kgraphics_set_pixel(int x, int y, uint32_t rgb) {
    int32_t *addr = kgraphics.buffer_base + sizeof(uint32_t)*(y*kgraphics.width + x);
    *addr = rgb | 0xff000000;
}

void kgraphics_fill_color(uint32_t rgb) {
    rgb |= 0xff000000;
    
    for (int y=0; y<kgraphics.height; y++) {
        for (int x=0; x<kgraphics.width; x++) {
            kgraphics_set_pixel(x, y, rgb);
        }
    }
}

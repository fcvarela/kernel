#pragma once

#include <stddef.h>
#include <stdint.h>

struct kgraphics {
    void*  buffer_base;
    size_t buffer_size, width, height;
};

extern struct kgraphics kgraphics;

void kgraphics_set_pixel(int w, int h, uint32_t rgb);
void kgraphics_fill_color(uint32_t rgb);

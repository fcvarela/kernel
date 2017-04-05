#pragma once

#include <stddef.h>
#include <stdint.h>

struct kgraphics {
  void *buffer_base;
  size_t buffer_size, width, height, px_per_line;
};

extern struct kgraphics kgraphics;

void kgraphics_set_pixel(uint32_t w, uint32_t h, uint32_t rgb);
void kgraphics_fill_color(uint32_t rgb);

// splash screen only
void kgraphics_draw_flag();

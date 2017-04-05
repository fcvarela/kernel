#include <kgraphics.h>

struct kgraphics kgraphics;

void kgraphics_set_pixel(uint32_t x, uint32_t y, uint32_t rgb) {
  uint32_t *addr = kgraphics.buffer_base + sizeof(uint32_t) * (y * kgraphics.px_per_line + x);
  *addr = rgb | 0xff000000;
}

void kgraphics_fill_color(uint32_t rgb) {
  rgb |= 0xff000000;
  for (uint32_t y = 0; y < kgraphics.height; y++) {
    for (uint32_t x = 0; x < kgraphics.width; x++) {
      kgraphics_set_pixel(x, y, rgb);
    }
  }
}

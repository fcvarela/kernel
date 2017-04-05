#include <kgraphics.h>
#include <kmain.h>
#include <kserial.h>

bool kgraphics_draw_flag_is_center_square(uint32_t x, uint32_t y) {
  bool x_inside_square = (x > kgraphics.width * 2 / 5 - kgraphics.width / 6) &&
                         (x < kgraphics.width * 2 / 5 + kgraphics.width / 6);

  bool y_inside_square = (y > kgraphics.height / 2 - kgraphics.width / 6) &&
                         (y < kgraphics.height / 2 + kgraphics.width / 6);

  return (x_inside_square && y_inside_square);
}

void kgraphics_draw_flag() {
  uint32_t red = 0xff << 16;
  uint32_t green = 0x66 << 8;
  uint32_t yellow = 0xff << 16 | 0xff << 8;

  for (uint32_t y = 0; y < kgraphics.height; y++) {
    for (uint32_t x = 0; x < kgraphics.width; x++) {
      // default red
      kgraphics_set_pixel(x, y, red);

      if (x < kgraphics.width * 2 / 5) {
        kgraphics_set_pixel(x, y, green);
      }

      // green?
      if (kgraphics_draw_flag_is_center_square(x, y)) {
        kgraphics_set_pixel(x, y, yellow);
      }
    }
  }
}

void kmain(void) {
  // set our pretty background
  kserial_print_string("Setting splash screen\n");
  kgraphics_draw_flag();

  kserial_print_string("Halting CPU\n");
  __asm__("hlt");
  for (;;) {
  }
}

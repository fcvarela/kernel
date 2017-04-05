#include <kgraphics.h>
#include <kmain.h>
#include <kserial.h>

inline void kmain(void) {
  // set our pretty background
  kserial_print_string("Setting splash screen\n");
  kgraphics_fill_color(0x23 << 16 | 0x2c << 8 | 0x31);

  kserial_print_string("Halting CPU\n");
  __asm__("hlt");
  for (;;) {
  }
}

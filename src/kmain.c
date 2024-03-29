#include <kgraphics.h>
#include <kmain.h>
#include <kserial.h>
#include <karch.h>

void kmain(void) {
  // set our pretty background
  kserial_print_string("Setting splash screen\n");
  kgraphics_draw_flag();

  kserial_print_string("Halting CPU\n");
  karch_halt();
}

#include <kmain.h>
#include <kgraphics.h>

void kmain(void) {
    // set our pretty background
    kgraphics_fill_color(0x23 << 16 | 0x2c << 8 | 0x31);

    __asm__("hlt");
    for (;;) {}
}

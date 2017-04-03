#include <kernel.h>

void kmain(void) {
    for (;;) {
        __asm__("hlt");
    }
}
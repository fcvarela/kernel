#include <kmemory.h>
#include <kserial.h>

struct kserial kserial;

void kserial_print_string(const char *string) {
  kserial_write((uint8_t *)string, kstrlen(string));
}

void kserial_print_int(uint64_t n) {
  char buf[24] = {0}, *bp = buf + 24;
  do {
    bp--;
    *bp = '0' + n % 10;
    n /= 10;
  } while (n != 0);
  kserial_write((uint8_t *)bp, buf - bp + 24);
}

void kserial_print_hex(uint64_t n) {
  char buf[16], *bp = buf + 16;

  for (int i = 0; i < 16; i++) {
    buf[i] = '0';
  }

  do {
    bp--;
    uint8_t mod = n % 16;
    if (mod < 10) {
      *bp = '0' + mod;
    } else {
      *bp = 'A' - 10 + mod;
    }
    n /= 16;
  } while (n != 0);
  kserial_write((uint8_t *)buf, 16);
}

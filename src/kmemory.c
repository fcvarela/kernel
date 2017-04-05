#include <kmemory.h>

uint64_t kstrlen(const char *data) {
  uint64_t r = 0;
  for (r = 0; *data != 0; data++, r++) {
  }
  return r;
}

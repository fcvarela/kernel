#pragma once

#include <stdint.h>
#include <stddef.h>

struct kserial {};

void kserial_print_string(const char *string);
void kserial_print_int(uint64_t n);
void kserial_print_hex(uint64_t n);

// provided by hardware (multiboot/efi)
uint64_t kserial_write(uint8_t *buffer, uint64_t size);
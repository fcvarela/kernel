#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct kserial {};

bool kserial_init();
void kserial_print_string(const char *string);
void kserial_print_int(uint64_t n);
void kserial_print_hex(uint64_t n);

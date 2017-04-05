#pragma once

#include <stdint.h>

uint8_t karch_port_outb(uint16_t port, uint8_t value);
uint8_t karch_port_inb(uint16_t port);

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <stddef.h>
#include <stdint.h>

struct kgraphics {
    void*  buffer_base;
    size_t buffer_size, width, height;
};

#ifdef BOOT_EFI
#include <efi.h>
#include <efilib.h>
#include <efiprot.h>

EFI_STATUS kgraphics_efi_init(EFI_SYSTEM_TABLE *st);
#endif

void kgraphics_set_pixel(int w, int h, uint32_t rgb);
void kgraphics_fill_color(uint32_t rgb);

#endif

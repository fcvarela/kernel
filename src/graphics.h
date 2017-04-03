#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <efi.h>
#include <efilib.h>
#include <efiprot.h>
#include <stddef.h>

struct graphics_info {
    EFI_GRAPHICS_OUTPUT_PROTOCOL         *protocol;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION output_mode;
    void*                                buffer_base;
    size_t                               buffer_size;
};

#define GRAPHICS_MOST_APPROPRIATE_H 768
#define GRAPHICS_MOST_APPROPRIATE_W 1024

EFI_STATUS select_mode(EFI_GRAPHICS_OUTPUT_PROTOCOL *graphics, OUT UINT32 *mode);
EFI_STATUS init_graphics(EFI_GRAPHICS_OUTPUT_PROTOCOL *graphics);

void set_pixel(int w, int h, uint32_t rgb);

#endif

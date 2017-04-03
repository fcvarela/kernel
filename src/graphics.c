#include <graphics.h>
#include <utils.h>

struct graphics_info graphics_info;

EFI_STATUS init_graphics(EFI_GRAPHICS_OUTPUT_PROTOCOL *graphics) {
    UINT32 new_mode;
    EFI_STATUS status = select_mode(graphics, &new_mode);
    ASSERT_EFI_STATUS(status, L"init_graphics");
    status = uefi_call_wrapper(graphics->SetMode, 2, graphics, new_mode);
    ASSERT_EFI_STATUS(status, L"init_graphics");
    graphics_info.protocol = graphics;
    graphics_info.buffer_base = (void*)graphics->Mode->FrameBufferBase;
    graphics_info.buffer_size = graphics->Mode->FrameBufferSize;

    return EFI_SUCCESS;
}

EFI_STATUS select_mode(EFI_GRAPHICS_OUTPUT_PROTOCOL *graphics, OUT UINT32 *mode) {
    *mode = graphics->Mode->Mode;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION most_appropriate_info;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN size;

    // Initialize info of current mode
    EFI_STATUS status = uefi_call_wrapper(graphics->QueryMode, 4, graphics, *mode, &size, &info);
    ASSERT_EFI_STATUS(status, L"select_mode");
    most_appropriate_info = *info;

    // Look for a better mode
    for(UINT32 i = 0; i < graphics->Mode->MaxMode; i += 1) {
        // Find out the parameters of the mode we’re looking at
        EFI_STATUS status = uefi_call_wrapper(graphics->QueryMode, 4,
                                              graphics, i, &size, &info);
        ASSERT_EFI_STATUS(status, L"select_mode");
        Print(L"Option #%d: w: %d, h: %d, pixel format: %d\n", i, info->HorizontalResolution, info->VerticalResolution, info->PixelFormat);
        // We only accept RGB or BGR 8 bit colorspaces.
        if(info->PixelFormat != PixelRedGreenBlueReserved8BitPerColor &&
           info->PixelFormat != PixelBlueGreenRedReserved8BitPerColor) {
            continue;
        }
        // If either of resolutions exceed appropriate w/h we cannot use the mode.
        if(info->HorizontalResolution > GRAPHICS_MOST_APPROPRIATE_W ||
           info->VerticalResolution > GRAPHICS_MOST_APPROPRIATE_H) {
            continue;
        }
        // Obviously the best mode!
        if(info->VerticalResolution == GRAPHICS_MOST_APPROPRIATE_H &&
           info->HorizontalResolution == GRAPHICS_MOST_APPROPRIATE_W) {
            most_appropriate_info = *info;
            *mode = i;
            break;
        }
        // Otherwise we have an arbitrary preferece to get as much vertical resolution as possible.
        if(info->VerticalResolution > most_appropriate_info.VerticalResolution) {
            most_appropriate_info = *info;
            *mode = i;
        }
    }
    graphics_info.output_mode = most_appropriate_info;
    Print(L"Option #%d selected: w: %d, h: %d, pixel format: %d\n",
          *mode,
          most_appropriate_info.HorizontalResolution, most_appropriate_info.VerticalResolution,
          most_appropriate_info.PixelFormat);
    return EFI_SUCCESS;
}

void set_pixel(int w, int h, uint32_t rgb) {
    w *= 4;
    h *= 4;
    int32_t *addr = graphics_info.buffer_base + w + h * graphics_info.output_mode.PixelsPerScanLine;
    *addr = rgb | 0xff000000;
}

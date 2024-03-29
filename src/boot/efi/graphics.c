#include "graphics.h"

#include <kgraphics.h>

// Initialize graphics and set display to largest supported mode
EFI_STATUS kgraphics_init(EFI_SYSTEM_TABLE *st) {
  // locate protocol
  EFI_GRAPHICS_OUTPUT_PROTOCOL *protocol = NULL;
  EFI_GUID gfx_proto = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_STATUS err = LibLocateProtocol(&gfx_proto, (void **)&protocol);
  if (err != EFI_SUCCESS) {
    Print(L"%r", err);
    return err;
  }

  // get current mode
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
  UINTN size;
  err = uefi_call_wrapper(protocol->QueryMode, 4, protocol, protocol->Mode->Mode, &size, &info);
  if (err != EFI_SUCCESS) {
    Print(L"%r", err);
    return err;
  }

  // Look for best mode (biggest height)
  UINT32 max_height = 0, mode = 0;
  for (UINT32 i = 0; i < protocol->Mode->MaxMode; i += 1) {
    err = uefi_call_wrapper(protocol->QueryMode, 4, protocol, i, &size, &info);
    Print(L"Found mode #%d: %dx%d (maxheight: %d)\n", i, info->HorizontalResolution,
          info->VerticalResolution, max_height);

    if (info->PixelFormat != PixelRedGreenBlueReserved8BitPerColor &&
        info->PixelFormat != PixelBlueGreenRedReserved8BitPerColor) {
      continue;
    }

    if (info->VerticalResolution > max_height) {
      max_height = info->VerticalResolution;
      mode = i;
    }
  }

  // refetch the selected mode
  err = uefi_call_wrapper(protocol->QueryMode, 4, protocol, mode, &size, &info);

  // copy settings to kgraphics
  kgraphics.buffer_base = (void *)protocol->Mode->FrameBufferBase;
  kgraphics.buffer_size = protocol->Mode->FrameBufferSize;
  kgraphics.width = info->HorizontalResolution;
  kgraphics.height = info->VerticalResolution;
  kgraphics.px_per_line = info->PixelsPerScanLine;

  // set it
  err = uefi_call_wrapper(protocol->SetMode, 2, protocol, mode);
  return EFI_SUCCESS;
}

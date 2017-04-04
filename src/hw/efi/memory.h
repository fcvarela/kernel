#pragma once

#include <efi.h>

struct efi_memorymap {
    EFI_MEMORY_DESCRIPTOR *map;
    UINTN                 map_size;
    UINTN                 map_key;
    UINTN                 descriptor_size;
    UINT32                descriptor_version;
};

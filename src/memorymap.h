#ifndef __MEMORYMAP_H__
#define __MEMORYMAP_H__

#include <efi.h>

struct efi_memorymap {
    EFI_MEMORY_DESCRIPTOR *map;
    unsigned long         map_size;
    unsigned long         desc_size;
    unsigned long         key;
    UINT32                desc_ver;
};

EFI_STATUS EFIAPI memorymap_sync(EFI_SYSTEM_TABLE *st, struct efi_memorymap *map);

#endif
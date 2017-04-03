#include <efi.h>
#include <efilib.h>

#include <kernel.h>
#include <memory.h>
#include <graphics.h>

extern struct kgraphics kgraphics;

EFI_STATUS EFIAPI efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *st) {
    struct efi_memorymap map;
    
    // EFI lib init
    InitializeLib(imageHandle, st);

    // Init graphics to graphics mode
    kgraphics_efi_init(st);

    // sync memory map and exit boot services, set address map
    map.map = LibMemoryMap(&map.map_size, &map.map_key, &map.descriptor_size, &map.descriptor_version);
    uefi_call_wrapper(st->BootServices->ExitBootServices, 2, imageHandle, map.map_key);
    uefi_call_wrapper(st->RuntimeServices->SetVirtualAddressMap, 4, map.map_size, map.descriptor_size, map.descriptor_version, map.map);
    
    // set our pretty background
    kgraphics_fill_color(0x23 << 16 | 0x2c << 8 | 0x31);
    
    // start kernel
    kmain();

    return EFI_SUCCESS;
}

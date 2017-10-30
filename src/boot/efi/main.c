#include <kmain.h>
#include <kserial.h>

#include "graphics.h"
#include "memory.h"

EFI_STATUS EFIAPI efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *st) {
  struct efi_memorymap memory_map;

  // EFI lib init
  InitializeLib(imageHandle, st);

  // Init serial for debugging and troubleshooting first
  kserial_init();

  // Init graphics to graphics mode
  kgraphics_init(st);

  // sync memory map
  memory_map.map = LibMemoryMap(&memory_map.map_size, &memory_map.map_key,
                                &memory_map.descriptor_size, &memory_map.descriptor_version);

  // and exit boot services, set address map
  uefi_call_wrapper(st->BootServices->ExitBootServices, 2, imageHandle, memory_map.map_key);
  uefi_call_wrapper(st->RuntimeServices->SetVirtualAddressMap, 4, memory_map.map_size,
                    memory_map.descriptor_size, memory_map.descriptor_version, memory_map.map);

  // start kernel
  kmain();

  return EFI_SUCCESS;
}

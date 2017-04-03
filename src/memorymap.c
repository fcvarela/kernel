#include <efi.h>
#include <efilib.h>

#include <memorymap.h>

const CHAR16 *memorymap_memorytypes[] = {
    L"EfiReservedMemoryType",
    L"EfiLoaderCode",
    L"EfiLoaderData",
    L"EfiBootServicesCode",
    L"EfiBootServicesData",
    L"EfiRuntimeServicesCode",
    L"EfiRuntimeServicesData",
    L"EfiConventionalMemory",
    L"EfiUnusableMemory",
    L"EfiACPIReclaimMemory",
    L"EfiACPIMemoryNVS",
    L"EfiMemoryMappedIO",
    L"EfiMemoryMappedIOPortSpace",
    L"EfiPalCode",
};

EFI_STATUS EFIAPI memorymap_sync(EFI_SYSTEM_TABLE *st, struct efi_memorymap *map) {
    EFI_MEMORY_DESCRIPTOR *m = NULL;
    UINT32 desc_version;
    unsigned long key;

    map->desc_size = sizeof(*m);
    map->map_size  = map->desc_size * 32;
    
    EFI_STATUS err;

again:
    // allocate pool
    err = uefi_call_wrapper((void *)st->BootServices->AllocatePool, 3, EfiLoaderData, map->map_size, (void **)&m);
    if (err != EFI_SUCCESS) {
        goto fail;
    }

    // this should fail with buffer too small
    err = uefi_call_wrapper((void *)st->BootServices->GetMemoryMap, 5, map->map_size, m, &key, map->desc_size, &desc_version);
    if (err == EFI_BUFFER_TOO_SMALL) {
        uefi_call_wrapper((void *)st->BootServices->FreePool, 1, m);
        map->map_size += map->desc_size * 8;
        goto again;
    }
    
    if (err != EFI_SUCCESS) {
        Print(L"Error in GetMemoryMap: %r\n", err);
        uefi_call_wrapper((void *)st->BootServices->FreePool, 1, m);
    }

    map->key = key;
    map->desc_ver = desc_version;

fail:
    map->map = m;
    return err;
}
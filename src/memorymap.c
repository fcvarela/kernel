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

UINT8 memorymap_map[MEMMAP_SIZE * sizeof(EFI_MEMORY_DESCRIPTOR)];

EFI_STATUS EFIAPI memorymap_sync(EFI_SYSTEM_TABLE *st) {
    UINTN memmap_size = MEMMAP_SIZE;
    UINTN descriptor_size;
    UINT32 descriptor_version;
    EFI_STATUS err = uefi_call_wrapper(st->BootServices->GetMemoryMap, 5, &memmap_size, (EFI_MEMORY_DESCRIPTOR*)memorymap_map, &memorymap_mapkey, &descriptor_size, &descriptor_version);
    if (err != EFI_SUCCESS) {
        Print(L"Error in memorymap_sync: %r\n", err);
        return err;
    }

    return err;
}
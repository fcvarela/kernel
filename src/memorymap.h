#ifndef __MEMORYMAP_H__
#define __MEMORYMAP_H__

#include <efi.h>

#define MEMMAP_SIZE 8192

EFI_STATUS EFIAPI memorymap_sync(EFI_SYSTEM_TABLE *st);

UINTN memorymap_mapkey;

#endif
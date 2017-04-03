#ifndef __SERIAL_H__
#define __SERIAL_H__

struct kserial {

};

#ifdef BOOT_EFI
#include <efi.h>
#include <efilib.h>

EFI_STATUS kserial_efi_init(EFI_SYSTEM_TABLE *st);
#endif

#endif
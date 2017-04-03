#include <serial.h>

struct kserial kserial;

#ifdef BOOT_EFI
EFI_STATUS kserial_efi_init(EFI_SYSTEM_TABLE *st) {
    return EFI_SUCCESS;
}
#endif

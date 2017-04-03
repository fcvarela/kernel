ARCH            = $(shell uname -m | sed s,i[3456789]86,ia32,)
INCLUDES        = $(wildcard src/*.h)
OBJS            = src/main_efi.o src/serial.o src/graphics.o src/kernel.o
TARGET          = kernel.efi
EFIINC          = /usr/include/efi
EFIINCS         = -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol -Isrc
LIB             = /usr/lib
EFILIB          = /usr/lib/gnuefi
EFI_CRT_OBJS    = $(LIB)/crt0-efi-$(ARCH).o
EFI_LDS         = $(LIB)/elf_$(ARCH)_efi.lds
CFLAGS          = $(EFIINCS) -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -Wall -DBOOT_EFI
ifeq ($(ARCH),x86_64)
  CFLAGS += -DEFI_FUNCTION_WRAPPER
endif
LDFLAGS         = -nostdlib -znocombreloc -T $(EFI_LDS) -shared -Bsymbolic -L $(EFILIB) -L $(LIB) $(EFI_CRT_OBJS)

all: $(TARGET)

$(OBJS): $(INCLUDES)

kernel.so: $(OBJS)
	ld $(LDFLAGS) $(OBJS) -o $@ -lefi -lgnuefi

%.efi: %.so
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .reloc --target=efi-app-$(ARCH) $^ $@
	rm $(OBJS)
#!/bin/bash

sudo qemu-system-x86_64 -cpu qemu64 -smp 2 -vga std -bios src/hwinit/efi/OVMF_X64.fd -usb -usbdevice disk:$1

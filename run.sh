#!/bin/bash

sudo qemu-system-x86_64 -cpu qemu64 -smp 2 -vga std -bios src/boot/efi/OVMF_X64.fd -serial stdio -usb -usbdevice disk:$1

#!/bin/bash

qemu-system-x86_64 -cpu qemu64 -smp 2 -vga std -bios src/intel-efi/OVMF.fd -usb -usbdevice disk:$1

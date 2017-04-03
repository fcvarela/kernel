#!/bin/bash

qemu-system-x86_64 -cpu qemu64 -smp 2 -bios src/intel-efi/OVMF.fd -usb -usbdevice disk:/dev/disk2

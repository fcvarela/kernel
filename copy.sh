#!/bin/bash -e

OS=`uname`

mkdir efimount

case $OS in
  'Linux')
    sudo mount $1 efimount 
    sudo cp kernel.efi efimount/EFI/BOOT/BOOTX64.EFI
    sudo umount efimount
    ;;
  'Darwin')
    diskutil mount -mountPoint efimount $1
    cp kernel.efi efimount/EFI/BOOT/BOOTX64.EFI
    diskutil unmount $1
    ;;
esac

rmdir efimount


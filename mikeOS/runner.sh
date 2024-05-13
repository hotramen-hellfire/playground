#!/bin/sh

# This script starts the QEMU PC emulator, booting from the
# MikeOS floppy disk image
qemu-system-i386 -audiodev pa,id=audio0 -machine pcspk-audiodev=audio0 -drive format=raw,file=disk_images/mikeos.flp

#!/bin/sh

# # Uncomment (ctrl+K+U in vscode) the following scripts to install all the prequisites

# # Grub

# tar xvzf grub-0.97-i386-pc.tar.gz

# # Build GRUB boot floppy

# dd if=/dev/zero of=a.img bs=512 count=2880
# sudo losetup /dev/loop27 a.img
# sudo dd if=./grub-0.97-i386-pc/boot/grub/stage1 of=/dev/loop27 bs=512 count=1
# sudo dd if=./grub-0.97-i386-pc/boot/grub/stage2 of=/dev/loop27 bs=512 seek=1
# sudo losetup -d /dev/loop27


# # Prepare disk image

# dd if=/dev/zero of=32M.img bs=4096 count=8192
# sudo losetup /dev/loop27 32M.img
# sudo fdisk /dev/loop27
# sudo losetup -d /dev/loop27
# sudo losetup -o 1048576 /dev/loop27 32M.img
# sudo mke2fs /dev/loop27
# sudo mount /dev/loop27 rootfs

# sudo mkdir rootfs/bootsudo 
# mkdir rootfs/boot/grub
# sudo cp ./grub-0.97-i386-pc/boot/grub/* rootfs/boot/grub

# sudo vim rootfs/boot/grub/menu.lst

# # Boot the hard disk with GRUB

# qemu-system-x86_64 -boot a -fda a.img -hda 32M.img
qemu-system-x86_64 -hda 32M.img
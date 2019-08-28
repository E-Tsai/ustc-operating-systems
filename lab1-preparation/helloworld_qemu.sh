#!/bin/sh

# # Uncomment (ctrl+K+U in vscode) the following scripts to install all the prequisites
# # Install requirements & compile linux kernel

# sudo apt-get install git fakeroot build-essential ncurses-dev xz-utils libssl-dev bc flex libelf-dev bison
# sudo apt-get install qemu-kvm qemu virt-manager virt-viewer libvirt-bin
# wget https://git.kernel.org/torvalds/t/linux-4.17-rc2.tar.gz
# tar xvzf linux-4.17-rc2.tar.gz
# cd linux-4.17-rc2
# make x86_64_defconfig
# make -j$(nproc)
# cd ..

# # Compile c file

# gcc -static hello.c -o init

# # Root dir

# dd if=/dev/zero of=myinitrd4M.img bs=4096 count=1024
# mke2fs myinitrd4M.img
# mkdir rootfs
# sudo mount -o loop myinitrd4M.img rootfs
# sudo cp init rootfs/.
# sudo mkdir rootfs/dev
# sudo mknod rootfs/dev/console c 5 1
# sudo mknod rootfs/dev/ram b 1 0

# # Reconfigure
cd linux-4.17-rc2
# make menuconfig
sudo make -j$(nproc)

# # Run on qemu
qemu-system-x86_64 -kernel arch/x86/boot/bzImage -initrd usr/initramfs_data.cpio.gz -append "root=/dev/ram init=/init"



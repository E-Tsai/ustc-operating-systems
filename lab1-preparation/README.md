# Lab 1: Preparation Lab

## Purpose of this lab

* Get a feeling of working on linux.
* Understand some basic OS concepts.
* Understand the **booting process of linux**.
* Understand some **gdb basics**.
* **Know how to google**.

## Content of this lab

* Install simulator qemu.
* Compile linux [kernel](https://en.wikipedia.org/wiki/Kernel_(operating_system)). 
* Make a simple initial root file system.
* Build a grub boot floppy.
* Learn how to use gdb on qemu.

## Instructions

### Run linux kernel on qemu

**Install qemu & compile linux kernel** 

Install requirements to compile linux kernel:

```shell
sudo apt-get install git fakeroot build-essential ncurses-dev xz-utils libssl-dev bc flex libelf-dev bison
```

Install qemu:

```shell
sudo apt-get install qemu-kvm qemu virt-manager virt-viewer libvirt-bin
```

Download the kernel.

```shell
wget https://git.kernel.org/torvalds/t/linux-4.17-rc2.tar.gz
```

Extracting the source.

```shell
tar xvzf linux-4.17-rc2.tar.gz
```

Switch to that dir, check out [configure, make and make install](https://thoughtbot.com/blog/the-magic-behind-configure-make-make-install) to get a better understanding of the following operations.

Check your system version:

```shell
uname -a
```

to see if it's 32-bit or 64-bit, then configure the kernel.

For 32-bit systems:

```shell
make i386_defconfig
```

For 64-bit systems:

```shell
make x86_64_defconfig
```

Then compile:

```shell
make -j$(nproc) 
```

You'll see 

```shell
Setup is 15548 bytes (padded to 15872 bytes).
System is 8061 kB
CRC c18c91cf
Kernel: arch/x86/boot/bzImage is ready  (#1)
```

When the compiling is finished. 

> [bzImage](https://en.wikipedia.org/wiki/Vmlinux#bzImage) means "big zImage". This format was developed to overcome this limitation by splitting the kernel over discontiguous memory regions. 

**Root file system** 

Prepare a simple c program (`hello.c`).

```c
#include<stdio.h>
int main(int argc,  const char ** argv){    
    printf("\033[1;32m");
    while(1){
        printf("Hello world!\n");
    }
    printf("\033[0m;");
    return 0;
}
```

Compile it into binary.

```shell
gcc -static hello.c -o init
```

Make a [root file system](https://developer.ibm.com/articles/l-initrd/).

> The *initial RAM disk (initrd)* is an initial root file system that is mounted prior to when the real root file system is available. The initrd is bound to the kernel and loaded as part of the kernel boot procedure. The kernel then mounts this initrd as part of the two-stage boot process to load the modules to make the real file systems available and get at the real root file system.

```shell
dd if=/dev/zero of=myinitrd4M.img bs=4096 count=1024
mke2fs myinitrd4M.img
mkdir rootfs
sudo mount -o loop myinitrd4M.img rootfs
```

> [/dev/zero](https://en.wikipedia.org/wiki//dev/zero): a special file in Unix-like operating systems that provides as many null characters (ASCII NUL, 0x00) as are read from it. One of the typical uses is to provide a character stream for initializing data storage.
>
> [dd](http://man7.org/linux/man-pages/man1/dd.1.html) - convert and copy a file. `of=FILE`: write to FILE instead of stdout. `if=FILE`: read from FILE instead of stdin. `bs=BYTES`: read and write up to BYTES bytes at a time (default: 512); overrides ibs and obs. `count=N`: copy only N input blocks. `seek=N`: skip N obs-sized blocks at start of output
>
> [mke2fs](https://linux.die.net/man/8/mke2fs) - create an ext2/ext3/ext4 filesystem.
>
> [mount](http://man7.org/linux/man-pages/man8/mount.8.html) - mount a filesystem.

Copy binary to target dir.

```shell
cp init rootfs/.
```

Make inode for console and ram.

```shell
sudo mkdir rootfs/dev
sudo mknod rootfs/dev/console c 5 1
sudo mknod rootfs/dev/ram b 1 0
```

> [mknod](https://www.ibm.com/support/knowledgecenter/en/ssw_aix_71/m_commands/mknod.html) - make a directory entry and corresponding [i-node](https://en.wikipedia.org/wiki/Inode) for a special file.
>
> [RAMDisk](http://memory.dataram.com/products-and-services/software/ramdisk) - a program that takes a portion of your system memory and uses it as a disk drive. `dev/ram` is a block device when the kernel boots. `dev/console` is the foreground virtual console, a character device. 

**Reconfigure linux kernel with our root dir**

Switch to kernel file.

```shell
make menuconfig
```

`General setup` -> `[*] Initial RAM filesystem and RAM disk (initramfs/initrd) support`
-> `Initramfs source file(s)` 

Enter the path of `rootfs`.

```shell
sudo make -j$(nproc) 
```

We'll have:

```shell
Kernel: arch/x86/boot/bzImage is ready  
```

Now we a kernel image and  `initramfs_data*` under `linux-4.17-rc2/usr`.

**Run Linux kernel on qemu**  

```shell
qemu-system-x86_64 -kernel arch/x86/boot/bzImage -initrd usr/initramfs_data.cpio.gz -append "root=/dev/ram rdinit=/init"
```

### [Boot with grub](https://www.linuxjournal.com/article/4622) 

> GRUB - GNU GRand Unified Bootloader.

**Install and compile grub** 

Download the `grub-0.97-i386-pc.tar.gz` from [here](ftp://alpha.gnu.org/gnu/grub/grub-0.97-i386-pc.tar.gz).

```shell
tar xvzf 
```

**Build a grub boot floppy** 

```shell
dd if=/dev/zero of=a.img bs=512 count=2880
```

Then:

```shell
sudo losetup /dev/loop3 a.img
sudo dd if=./grub-0.97-i386-pc/boot/grub/stage1 of=/dev/loop3 bs=512 count=1
sudo dd if=./grub-0.97-i386-pc/boot/grub/stage2 of=/dev/loop3 bs=512 seek=1
sudo losetup -d /dev/loop3
```

> [losetup](https://www.computerhope.com/unix/losetup.htm)  - associate loop devices with regular files or [block devices](https://en.wikipedia.org/wiki/Device_file#Block_devices), to detach loop devices and to query the status of a [loop device](https://en.wikipedia.org/wiki/Loop_device).
>
> The Stage 1 or primary boot loader is read into memory by the BIOS from the [MBR (Master Boot Record)](https://en.wikipedia.org/wiki/Master_boot_record) . The primary boot loader exists on less than 512 bytes of disk space within the MBR and is capable of loading either the Stage 1.5 or Stage 2 boot loader.

Test if we can enter grub GUI:

```shell
qemu-system-x86_64 -fda a.img
```

**Prepare disk image**:

```shell
dd if=/dev/zero of=32M.img bs=4096 count=8192
sudo losetup /dev/loop3 32M.img
```

Add a new partition for 32M.img and set it as MBR.

```shell
sudo fdisk /dev/loop3
```

> fdisk - a command-line utility that provides disk partitioning functions. **p**: print the partition table; **n**: create a new partition; **d**: delete a partition; **q**: quit without saving changes; **w**: write the new partition table and exit.

Detach 32M.img

```shell
sudo losetup -d /dev/loop3
```



```shell
sudo losetup -o 32256 /dev/loop3 32M.img
```



```shell
sudo mke2fs /dev/loop28
```

You'll see something like:

```shell
mke2fs 1.44.1 (24-Mar-2018)
Discarding device blocks: done                            
Creating filesystem with 32736 1k blocks and 8192 inodes
Filesystem UUID: 7e718849-86b6-4500-a741-28f1b167d272
Superblock backups stored on blocks: 
	8193, 24577

Allocating group tables: done                            
Writing inode tables: done                            
Writing superblocks and filesystem accounting information: done
```

```shell
sudo mount /dev/loop3 rootfs
```



```shell
sudo mkdir rootfs/boot
sudo mkdir rootfs/boot/grub
sudo cp ./grub-0.97-i386-pc/boot/grub/* rootfs/boot/grub
```



```shell
default 0
timeout 30
title linux on 32M.img
root (hd0,0)
kernel (hd0,0)/bzImage root=/dev/ram init=/bin/ash
initrd (hd0,0)/initramfs_data.cpio.gz
```



```shell
qemu-system-x86_64 -boot a -fda a.img -hda 32M.img
```



## References

[What is a kernel](https://en.wikipedia.org/wiki/Kernel_(operating_system)) 

[Linux initial RAM disk (initrd) overview](https://developer.ibm.com/articles/l-initrd/) 

[What is /dev/zero](https://en.wikipedia.org/wiki//dev/zero) 

[Other pseudo devices](https://en.wikipedia.org/wiki/Device_file#Pseudo-devices) 

[The dd command](http://man7.org/linux/man-pages/man1/dd.1.html) 

[The mke2fs command](https://linux.die.net/man/8/mke2fs) 

[The mount command](http://man7.org/linux/man-pages/man8/mount.8.html) 

[The mknod command](https://www.ibm.com/support/knowledgecenter/en/ssw_aix_71/m_commands/mknod.html) 

[What is an i-node](https://en.wikipedia.org/wiki/Inode) 

[What is bzImage](https://en.wikipedia.org/wiki/Vmlinux#bzImage) 

[What is RAMDisk](http://memory.dataram.com/products-and-services/software/ramdisk) 

[Boot with grub](https://www.linuxjournal.com/article/4622) 

[The magic behind configure, make, make install](https://thoughtbot.com/blog/the-magic-behind-configure-make-make-install) 

[The losetup command](https://www.computerhope.com/unix/losetup.htm) 

[What are block devices](https://en.wikipedia.org/wiki/Device_file#Block_devices) 

[What is a loop device](https://en.wikipedia.org/wiki/Loop_device) 

[How to use loop devices](https://blog.sleeplessbeastie.eu/2017/07/03/how-to-use-loop-devices/) 

[The fdisk command](https://en.wikipedia.org/wiki/Fdisk) 

[Partitioning with fdisk](https://www.tldp.org/HOWTO/Partition/fdisk_partitioning.html) 

[How does Grub Stage1, exactly, access/load stage 2?](https://unix.stackexchange.com/questions/259143/how-does-grub-stage1-exactly-access-load-stage-2) 

[MBR (Master Boot Record)](https://en.wikipedia.org/wiki/Master_boot_record) 
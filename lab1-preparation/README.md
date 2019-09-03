# Lab 1: Getting Ready

*Warm-up. Compile a Linux kernel and create a root file system. Create boot disk with GRUB to run on QEMU. Learn GDB basics.*

## **Purpose**‌

Participants are expected to:

- Get a feeling of working on Linux.
- Understand some basic OS concepts, and be able to answer the following questions:
  * [What is a kernel?](https://en.wikipedia.org/wiki/Kernel_(operating_system)) 
  * [What is the initial ramdisk? ](https://en.wikipedia.org/wiki/Initial_ramdisk) 
  * [What is a boot disk?](https://en.wikipedia.org/wiki/Boot_disk) 
  * [What are block devices?](https://en.wikipedia.org/wiki/Device_file#Block_devices) 
  * [What is a loop device?](https://en.wikipedia.org/wiki/Loop_device) 
  * [What is BIOS?](https://en.wikipedia.org/wiki/BIOS) 
  * [What is MBR?](https://en.wikipedia.org/wiki/Master_boot_record) 
- Understand the **booting process of Linux kernel**.
- Learn some **GDB basics**.
- **Know how to google**.

...through the process of completing this warm-up experiment.‌

## **Contents**

- Install [QEMU](https://www.qemu.org/), the hard virtualization emulator. 
- Compile Linux [kernel](https://en.wikipedia.org/wiki/Kernel_(operating_system)).
- Make a simple [initial root file system](https://en.wikipedia.org/wiki/Initial_ramdisk).
- Create a GRUB [boot disk](https://en.wikipedia.org/wiki/Boot_disk).
- Learn how to use [GDB](https://www.tutorialspoint.com/gnu_debugger/what_is_gdb.htm) on QEMU.

## **Instructions**

*Participants of this course are not required to have programming experience on Linux. We understand that there might be an information overload and will try our best to explain all the new concepts along the way. However, we strongly encourage and recommend you to take a deep dive and answer your own questions that occur in the process of completing this warm-up tutorial.*

### **Run Linux kernel on QEMU**

**Install QEMU & compile Linux kernel**

Install requirements to compile Linux kernel:

```shell
sudo apt-get install git fakeroot build-essential ncurses-dev xz-utils libssl-dev bc flex libelf-dev bison
```

Install QEMU:

```shell
sudo apt-get install qemu-kvm qemu virt-manager virt-viewer libvirt-bin
```

Download a Linux kernel (Any stable releases are feasible, yet we recommend you to download one with kernel version later than 4+ to avoid tool chain compatibility issues):

```shell
wget https://git.kernel.org/torvalds/t/linux-4.17-rc2.tar.gz
```

Extract the source:

```shell
tar xvzf linux-4.17-rc2.tar.gz
```

Switch to the kernel directory, check out [configure, make and make install](https://thoughtbot.com/blog/the-magic-behind-configure-make-make-install) to get a better understanding of the following operations.

Then configure the kernel.

To compile a 32-bit system:

```shell
make i386_defconfig
# use qemu-system-i386 later
```

To compile a 64-bit system:

```shell
make x86_64_defconfig	# we're using this configuration
# use qemu-system-x86_64 later
```

Compile the kernel:

```shell
make -j$(nproc) 
```

*hint: What does -j$(nproc) mean?* 

You'll see something like:

```shell
Setup is 15548 bytes (padded to 15872 bytes).
System is 8061 kBCRC c18c91cf
Kernel: arch/x86/boot/bzImage is ready  (#1)
```

...when the compiling is finished.

> [bzImage](https://en.wikipedia.org/wiki/Vmlinux#bzImage) means "big zImage". This format was developed to overcome this limitation by splitting the kernel over discontiguous memory regions.

**Initial Root File System**‌

Prepare a simple c program (`hello.c`).

```C
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

‌Compile it into binary:

```shell
gcc -static hello.c -o init
```

Create a [initrd](https://developer.ibm.com/articles/l-initrd/):

> The *initial RAM disk (initrd)* is an initial root file system that is mounted prior to when the real root file system is available. The initrd is bound to the kernel and loaded as part of the kernel boot procedure. The kernel then mounts this initrd as part of the two-stage boot process to load the modules to make the real file systems available and get at the real root file system.

```shell
dd if=/dev/zero of=myinitrd4M.img bs=4096 count=1024
mke2fs myinitrd4M.img
mkdir rootfs
sudo mount -o loop myinitrd4M.img rootfs
```

> [dd](http://man7.org/linux/man-pages/man1/dd.1.html) - convert and copy a file. 
>
> arguments of the dd command: `of=FILE`: write to FILE instead of stdout. `if=FILE`: read from FILE instead of stdin. `bs=BYTES`: read and write up to BYTES bytes at a time (default: 512); overrides ibs and obs. `count=N`: copy only N input blocks. `seek=N`: skip N obs-sized blocks at start of output
>
> [/dev/zero](https://en.wikipedia.org/wiki//dev/zero): a special file in Unix-like operating systems that provides as many null characters (ASCII NUL, 0x00) as are read from it. One of the typical uses is to provide a character stream for initializing data storage.
>
> [mke2fs](https://linux.die.net/man/8/mke2fs) - create an ext2/ext3/ext4 filesystem.
>
> [mount](http://man7.org/linux/man-pages/man8/mount.8.html) - mount a filesystem.

Copy binary to target directory:

```shell
cp init rootfs/.
```

Create console and ram disk for the initrd and corresponding i-nodes: 

```shell
sudo mkdir rootfs/dev
sudo mknod rootfs/dev/console c 5 1
sudo mknod rootfs/dev/ram b 1 0
```

> [mknod](https://www.ibm.com/support/knowledgecenter/en/ssw_aix_71/m_commands/mknod.html) - make a directory entry and corresponding [i-node](https://en.wikipedia.org/wiki/Inode) for a special file.
>
> [RAMDisk](http://memory.dataram.com/products-and-services/software/ramdisk) - a program that takes a portion of your system memory and uses it as a disk drive. `dev/ram` is a block device when the kernel boots. `dev/console` is the foreground virtual console, a character device.

**Reconfigure Linux kernel with our new initial RAM file system**

Switch to the kernel directory. Modify configuration information:

```shell
make menuconfig
```

`‌General setup` -> `[*] Initial RAM filesystem and RAM disk (initramfs/initrd) support` -> `Initramfs source file(s)`

Enter the path of `rootfs`. 

*hint: What's the purpose of the modifying `menuconfig`?*

Recompile the kernel:

```shell
sudo make -j$(nproc) 
```

We'll have:

```shell
Kernel: arch/x86/boot/bzImage is ready  
```

Now we a kernel image and `initramfs_data*` under `linux-4.17-rc2/usr`.

*hint: What is this `initramfs_data.cpio.gz` ?‌*

**Run Linux kernel on QEMU**

```shell
qemu-system-x86_64 -kernel arch/x86/boot/bzImage -initrd usr/initramfs_data.cpio.gz -append "root=/dev/ram init=/init"
```

## **Boot with GRUB**

> GRUB - GNU GRand Unified Bootloader.

**Install and compile GRUB**

Download `grub-0.97-i386-pc.tar.gz` from [here](ftp://alpha.gnu.org/gnu/grub/grub-0.97.tar.gz), or the download the [compiled version](ftp://alpha.gnu.org/gnu/grub/grub-0.97-i386-pc.tar.gz). Extract GRUB:

```shell
tar xvzf grub-0.97-i386-pc.tar.gz
```

In this section, we're using the traditional [BIOS](https://en.wikipedia.org/wiki/BIOS)-[MBR](https://en.wikipedia.org/wiki/Master_boot_record) method for power-on startup.

> BIOS (Basic Input/Output System): non-volatile firmware used to perform hardware initialization during the booting process.
>
> MBR (Master Boot Record): a special type of boot sector at the very beginning of partitioned computer mass storage devices.

The boot process is broken down into several major components, each of which is a completely-separate subsystem with many different options and variations:

* BIOS: The PC is turned on & the BIOS initializes the hardware.
* MBR: The BIOS calls code stored in the MBR at the start of disk 0.
* Active Partition: The MBR loads code from the bootsector of the active partition.
* Bootloader: The bootsector loads & runs the bootloader from its file systems.

For further reading, please check out [The BIOS/MBR Boot Process](https://neosmart.net/wiki/mbr-boot-process/).

**Build a GRUB boot floppy:**

```shell
dd if=/dev/zero of=a.img bs=512 count=2880
```

*hint: Why the default buffer size of `bs` is 512 bytes?‌*

Display first found loop device:

```shell
$ losetup -f
/dev/loop<num>
```

Convert and copy boot loader information from GRUB to `a.img`:

```shell
sudo losetup /dev/loop<num> a.img
sudo dd if=./grub-0.97-i386-pc/boot/grub/stage1 of=/dev/loop<num> bs=512 count=1
sudo dd if=./grub-0.97-i386-pc/boot/grub/stage2 of=/dev/loop<num> bs=512 seek=1
sudo losetup -d /dev/loop<num>
```

> [losetup](https://www.computerhope.com/unix/losetup.htm) - associate loop devices with regular files or [block devices](https://en.wikipedia.org/wiki/Device_file#Block_devices), to detach loop devices and to query the status of a [loop device](https://en.wikipedia.org/wiki/Loop_device).
>
> The Stage 1 or primary boot loader is read into memory by the BIOS from the [MBR (Master Boot Record)](https://en.wikipedia.org/wiki/Master_boot_record) . The primary boot loader exists on less than 512 bytes of disk space within the MBR and is capable of loading either the Stage 1.5 or Stage 2 boot loader.

**Test if we can enter GRUB GUI**:

```shell
qemu-system-x86_64 -fda a.img
```

**Prepare disk image**:

```shell
dd if=/dev/zero of=32M.img bs=4096 count=8192
sudo losetup /dev/loop<num> 32M.img
```

‌Create a new partition for 32M.img and set it as MBR.

```shell
sudo fdisk /dev/loop<num>
```

> fdisk - a command-line utility that provides disk partitioning functions. **p**: print the partition table; **n**: create a new partition; **d**: delete a partition; **q**: quit without saving changes; **w**: write the new partition table and exit.

*hint: What to do after the warning "The kernel still uses the old table. The new table will be used at the next reboot or after you run partprobe(8) or kpartx(8)." popped up?*

Detach 32M.img.

```shell
sudo losetup -d /dev/loop<num>
```

Get startsector info.

```shell
file 32M.img
```

You'll see something like:

```shell
32M.img: DOS/MBR boot sector; partition 1 : ID=0x83, start-CHS (0x0,32,33), 
end-CHS (0x4,20,16), startsector 2048, 63488 sectors, extended partition table (last)
```

Use this information to move data start at startsector of our MBR.

```shell
sudo losetup -o 1048576 /dev/loop<num> 32M.img
```

> 1048576 is the starting position of the partition, which is 2048 * 512.

 **Format the partition to ext2fs.**

```shell
sudo mke2fs /dev/loop<num>
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

‌Mount it to `rootfs`.

```shell
sudo mount /dev/loop<num> rootfs
```

Add `/boot `and `/grub` directory entry.

```shell
sudo mkdir rootfs/bootsudo 
mkdir rootfs/boot/grub
sudo cp ./grub-0.97-i386-pc/boot/grub/* rootfs/boot/grub
```

Create file `rootfs/boot/grub/menu.lst`.

```shell
default 0
timeout 30
title linux on 32M.img
root (hd0,0)
kernel (hd0,0)/bzImage root=/dev/ram init=/bin/ash
initrd (hd0,0)/initramfs_data.cpio.gz
```

*hint: What is the purpose of the GRUB main menu list?*

**Boot the floppy with GRUB:**

```shell
qemu-system-x86_64 -boot a -fda a.img -hda 32M.img
```

*hint: What are the roles of `a.img` and `32M.img`?*

**Add GRUB function to the disk image:**

```shell
>grub root (hd0,0)
>grub setup (hd0)
```

*hint: What does (hdx, y) mean?*

**Boot the hard disk with GRUB:**

```shell
qemu-system-x86_64 -hda 32M.img
```

*hint: What is the difference between booting from floppy and hard disk?*

### **Use GDB with QEMU**

*If you've never used GDB before, you might want to check out* [*this GDB tutorial*](https://web.eecs.umich.edu/~sugih/pointers/summary.html)*.*

You can run QEMU to listen for a "GDB connection" before it starts executing any code to debug it. Switch to the Linux kernel directory.

```shell
qemu-system-x86_64 -kernel arch/x86/boot/bzImage -s -S
```

...will setup QEMU to listen on port 1234 and wait for a GDB connection to it. 

>  -S freezes CPU at startup (use 'c' to start execution) 
>
>  -s stands for `-gdb tcp::1234` 

Then, from another local shell:

```shell
gdb 
(gdb) file vmlinux
```

...[vmlinux](https://en.wikipedia.org/wiki/Vmlinux) is what we will be using during debugging of the Linux kernel.

> **vmlinux** -  a statically linked executable file that contains the Linux kernel in one of the object file formats supported by Linux.

```shell
(gdb) target remote:1234 
```

Set break point:

```shell
(gdb) b start_kernel
(gdb) c
```

...it will stop when reached `start_kernel`, waiting for next GDB command.‌

**Reconfigure Linux kernel and compile it with debugging information**:

```shell
make menuconfig
```

‌`kernel hacking` -> `[*] compile the kernel with debug info`

Recompile it:

```shell
make -j$(nproc) 
```

Now you can list source code at GDB breakpoints or use

```shell
(gdb) info registers
```

...to view registers at anytime with this command.

*Congratulations! You have finished the warm-up tutorial :)*

# **References** 

[What is QEMU?](https://www.qemu.org/) 

[What is a kernel?](https://en.wikipedia.org/wiki/Kernel_(operating_system)) 

[Linux initial RAM disk (initrd) overview](https://developer.ibm.com/articles/l-initrd/) 

[What is the initial ramdisk? ](https://en.wikipedia.org/wiki/Initial_ramdisk) 

[What is a boot disk?](https://en.wikipedia.org/wiki/Boot_disk) 

[What is GDB?](https://www.tutorialspoint.com/gnu_debugger/what_is_gdb.htm) 

[What is /dev/zero?](https://en.wikipedia.org/wiki//dev/zero) 

[Other pseudo devices](https://en.wikipedia.org/wiki/Device_file#Pseudo-devices) 

[The dd command](http://man7.org/linux/man-pages/man1/dd.1.html) 

[The mke2fs command](https://linux.die.net/man/8/mke2fs) 

[The mount command](http://man7.org/linux/man-pages/man8/mount.8.html) 

[The mknod command](https://www.ibm.com/support/knowledgecenter/en/ssw_aix_71/m_commands/mknod.html) 

[What is an i-node?](https://en.wikipedia.org/wiki/Inode) 

[What is bzImage?](https://en.wikipedia.org/wiki/Vmlinux#bzImage) 

[What is RAMDisk?](http://memory.dataram.com/products-and-services/software/ramdisk) 

[Boot with grub](https://www.linuxjournal.com/article/4622) 

[The magic behind configure, make, make install](https://thoughtbot.com/blog/the-magic-behind-configure-make-make-install) 

[The losetup command](https://www.computerhope.com/unix/losetup.htm) 

[What are block devices?](https://en.wikipedia.org/wiki/Device_file#Block_devices) 

[What is a loop device?](https://en.wikipedia.org/wiki/Loop_device) 

[How to use loop devices?](https://blog.sleeplessbeastie.eu/2017/07/03/how-to-use-loop-devices/) 

[The fdisk command](https://en.wikipedia.org/wiki/Fdisk) 

[Partitioning with fdisk](https://www.tldp.org/HOWTO/Partition/fdisk_partitioning.html) 

[How does Grub Stage1, exactly, access/load stage 2?](https://unix.stackexchange.com/questions/259143/how-does-grub-stage1-exactly-access-load-stage-2) 

[MBR (Master Boot Record)](https://en.wikipedia.org/wiki/Master_boot_record)  

[GRUB: how find partition number (hd0,x)?](https://superuser.com/questions/182161/grub-how-find-partition-number-hd0-x) 

[Use GDB with QEMU](https://wiki.osdev.org/Kernel_Debugging#Use_GDB_with_QEMU) 

[What is vmlinux?](https://en.wikipedia.org/wiki/Vmlinux) 

[The GNU **objcopy** utility](https://linux.die.net/man/1/objcopy) 

[Kernel Debugging](https://wiki.osdev.org/Kernel_Debugging) 

[A GDB tutorial](https://web.eecs.umich.edu/~sugih/pointers/summary.html) 

[What is BIOS?](https://en.wikipedia.org/wiki/BIOS) 

[What is MBR?](https://en.wikipedia.org/wiki/Master_boot_record) 

[The BIOS/MBR Boot Process](https://neosmart.net/wiki/mbr-boot-process/) 


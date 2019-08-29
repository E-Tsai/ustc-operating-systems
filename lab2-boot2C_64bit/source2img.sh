#!/bin/sh

IMG_NAME=output/a_boot2C.img
#LOOP_DEV=`sudo losetup -f`
#if [ -z "${LOOP_DEV}" ]; then
#	echo "Failed to find a loop device, please check..."
#	exit
#fi

echo "Compiling into Binary..."
echo "make clean"
make clean

echo "make"
make

echo "Building boot floppy..."
sudo dd if=output/start16.bin of=${IMG_NAME} bs=512 count=1
sudo dd if=output/myOS.bin of=${IMG_NAME} bs=512 seek=1

echo "qemu-system-i386 -fda ${IMG_NAME}"
sudo qemu-system-i386 -fda ${IMG_NAME}

#echo "dd if=/dev/zero of=" ${IMG_NAME} "bs=512 count=2880"
#dd if=/dev/zero of=output/a_boot2C.img bs=512 count=2880

#echo "sudo losetup ${LOOP_DEV} output/a_boot2C.img"
#sudo losetup ${LOOP_DEV} output/a_boot2C.img

#echo "sudo dd if=output/start16.bin of=${LOOP_DEV} bs=512 count=1"
#sudo dd if=output/start16.bin of=${LOOP_DEV} bs=512 count=1


#echo "sudo dd if=output/myOS.bin of=${LOOP_DEV} bs=512 seek=1"
#sudo dd if=output/myOS.bin of=${LOOP_DEV} bs=512 seek=1

#echo "sudo losetup -d ${LOOP_DEV}"
#sudo losetup -d ${LOOP_DEV}

BOOTSECT=bootsect/start16
BOOTSECT_OBJS=output/${BOOTSECT}.o
BOOTSECT_BIN=output/${BOOTSECT}.bin

output/${BOOTSECT}.o: ${BOOTSECT}.S
	gcc -c ${ASM_FLAGS} ${BOOTSECT}.S -o output/${BOOTSECT}.o	

output/${BOOTSECT}.bin: ${BOOTSECT_OBJS}
	ld -T ${BOOTSECT}.ld ${BOOTSECT_OBJS} -o output/${BOOTSECT}.elf
	objcopy -O binary output/${BOOTSECT}.elf output/${BOOTSECT}.bin
	cp output/${BOOTSECT}.bin output/start16.bin

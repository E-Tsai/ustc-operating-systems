ASM_FLAGS= -m32 --pipe -Wall -fasm -g -O1 -fno-stack-protector
C_FLAGS = -m32 -fno-stack-protector -g

all: myOS
include bootsect/Makefile
include myOS/Makefile
include userApp/Makefile

OS_OBJS       = ${MYOS_OBJS} ${USER_APP_OBJS}

myOS: ${BOOTSECT_BIN} ${OS_OBJS}
	ld -T myOS/myOS.ld ${OS_OBJS} -o output/myOS.elf
	objcopy -O binary output/myOS.elf output/myOS.bin
	
clean:
	rm -f output/*.* output/userApp/*.* output/myOS/*.* output/bootsect/*.*

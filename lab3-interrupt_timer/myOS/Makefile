MYOS_OBJS     = output/myOS/start32.o output/myOS/task.o output/myOS/timer.o output/myOS/myPrintf.o

output/myOS/myPrintf.o: myOS/myPrintf.c
	gcc -c ${C_FLAGS} myOS/myPrintf.c -o output/myOS/myPrintf.o

output/myOS/timer.o: myOS/timer.c
	gcc -c ${C_FLAGS} myOS/timer.c -o output/myOS/timer.o

output/myOS/task.o: myOS/task.c
	gcc -c ${C_FLAGS} myOS/task.c -o output/myOS/task.o

output/myOS/start32.o: myOS/start32.S
	gcc -c ${ASM_FLAGS} myOS/start32.S -o output/myOS/start32.o

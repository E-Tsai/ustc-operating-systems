# Lab 2: Starting From Boot Loader

*A simple OS with a FCFS task manager.*

## Purpose

In the following experiments, we are going to build a simple bootable operating system. In this experiment, participants are expected to:

* Understand the following OS concepts:
  * [real mode](https://en.wikipedia.org/wiki/Real_mode), [protected mode](https://en.wikipedia.org/wiki/Protected_mode);
  * [BIOS interrupt call](https://en.wikipedia.org/wiki/BIOS_interrupt_call);
  * [Bootloader](https://en.wikipedia.org/wiki/Booting#First-stage_boot_loader);
  * [Global Descriptor Table (GDT)](https://en.wikipedia.org/wiki/Global_Descriptor_Table);
  * [Task Control Block](https://en.wikipedia.org/wiki/Task_Control_Block)/[Process control block](https://en.wikipedia.org/wiki/Process_control_block);
  * [Multitasking](https://en.wikipedia.org/wiki/Computer_multitasking);
  * [Stack-based memory allocation](https://en.wikipedia.org/wiki/Stack-based_memory_allocation);
  * [Data segment structure](https://en.wikipedia.org/wiki/Data_segment);
  * [Context switch](https://en.wikipedia.org/wiki/Context_switch);
* Understand the provided code structure and be able to answer the following questions:
  * What is a [compiler](https://en.wikipedia.org/wiki/Compiler)? How does it work?
  * What is a [Makefile](https://opensource.com/article/18/8/what-how-makefile)? How does it work?
  * What is a [linker](https://en.wikipedia.org/wiki/GNU_linker)? How does it work?
  * What is a [Executable and Linkable Format (ELF)](https://elinux.org/Executable_and_Linkable_Format_(ELF))?
  * What is a [IMG](https://en.wikipedia.org/wiki/IMG_(file_format))?

## Contents

* Read following source code.
  * `bootsect/start16.S`;
  * `bootsect/start16.ld`;
  * `myOS/start32.S`;
  * `myOS/start32.ld`; 
  * `userApp/main.c`;
  * All 4 Makefiles;
* Compile source code into binary.
* Build a boot floppy.
* Run with QEMU:
  
  * file `source2img.sh`: the shell script to generate all binaries and run the boot images on QEMU.
  
  We provide a script `source2img.sh` in the root directory which includes all operations. However, do not simply run it without having a look.

## Requirements

* In `myOS/start32.S`, add assembly code for context switch;

* In `myOS/`, add `task.c` or other C files for `osStart()` (task management/scheduling);

* In `myOS/Makefile`, add rules to compile newly added files;

* In `myOS/`, add `myOS.h` (this file name cannot be changed) to list following interfaces between our operating system  `userApp`:

  1. Function: `void osStart(void);` 

     Defined and implemented by operating system. 

     Initialize a task manager ([FCFS](https://www.techopedia.com/definition/23455/first-come-first-served-fcfs)) for the operating system. Create `idle task` and `init task`. Enter multitasking mode.

  2. Function: `int createTsk(void (*tskBody)(void));` 

     Defined and implemented by operating system.

     Create a task and set its state as "ready".

  3. Function: `void tskEnd(void);`

     Defined and implemented by operating system.

     Destroy finished tasks and switch context.

  4. Function: `void initTskBody(void);` 

     Defined by operating system, implemented by `userApp`.

     Create a task body for `init task`, which includes other tasks from  `userApp`.

  5. Macro: `USER_TASK_NUM` 

     \>= 1; Defined in `userApp/userApp.h`. 

     Provide the total amount of `userApp` tasks.

  ...and implement these interfaces.

* Replace old `userApp` with new provided testing code.

* Provide a code diagram for this project.

## Instructions

### Environment

* Compiling tools: gcc (>=4.8), ld;
* Emulator: qemu-system-i386 (32-bit).

### Code structure

```shell
├── bootsect
│   ├── start16.ld		# linker for the boot secter
│   ├── Makefile
│   └── start16.S		# asm code for the boot secter
├── Makefile
├── myOS
│   ├── myOS.ld			# linker script for myOS
│   ├── Makefile
│   ├── myOS.h
│   ├── start32.S		# assembly code for our simple OS
|	|					# calls userApp/main.c:myMain()
│   └── task.c
├── output
└── userApp
    ├── init.c  		# contains iniTsk and 
    |					# 3 simple print message functions	
    ├── Makefile
    ├── userApp.h
    └── main.c			# provide myMain(), which prints 
    					# "hello world" in this experiment.
```

For those who haven't used Makefile before, please check out this [simple Makefile tutorial](http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/). 

*hint: add `mkdir -p <filename>` in Makefile can avoid some "No such file or directory" error.*

We provide a compilation diagram of this project:

```
   bootsect/                    myOS/                  userApp/          
-------------------------------------------------------------------------- 
   start16.S            start32.S    task.c        init.c    main.c 
       |                    |          |             |         |   
       |                    |          |             |         |          
   start16.o            start32.o    task.o        init.o    main.o           
       |                    |          |             |         |            
       | start16.ld         |          |             |         |            
       |                    -----------------|------------------  
  start16.elf                                | myOS.ld        
       |                                  myOS.elf          
       |                                     |                
  start16.bin                             myOS.bin      
       \                                      /              
        \                                    /             
         \             bootloader           /            
        +------------------------|-----------------------+                    
        |          MBR           |          OS           |  a_boot2C.img  
        +------------------------------------------------+             
       0x7C00                   0x7E00                                                     
                                                                   
```

However, we leave it to you to understand the details of the compilation.           

> [Linker](https://en.wikipedia.org/wiki/GNU_linker): creates an [executable file](https://en.wikipedia.org/wiki/Executable_file) (or a [library](https://en.wikipedia.org/wiki/Library_(software))) from [object files](https://en.wikipedia.org/wiki/Object_file) created during [compilation](https://en.wikipedia.org/wiki/Compiler) of a software project.
>
> Computer programs typically are composed of several parts or modules; these parts/modules need not all be contained within a single object file, and in such cases refer to each other by means of symbols as addresses into other modules, which are mapped into memory addresses when linked for execution.         
>
> [objdump](https://en.wikipedia.org/wiki/Objdump): a command-line program for displaying various information about [object files](https://en.wikipedia.org/wiki/Object_file). For instance, it can be used as a [disassembler](https://en.wikipedia.org/wiki/Disassembler) to view an [executable](https://en.wikipedia.org/wiki/Executable) in assembly form.                          

### Walk through

**Boot Loader Stage 1**

Though the boot sector and the transition from real mode to protected mode is provided, we encourage you to take a look at the source code (including the linker) to understand what happens there.

The stage 1 of the boot loader usually includes the following steps (in order of execution):

* Hardware device initialization.
* Prepare RAM space for loading the boot loader's stage2.
* Copy the stage 2 of the Boot Loader into RAM space.
* Set up the stack.
* Jump to the C entry point of stage 2.

In `bootsect/start16.S`, we start the OS from boot sector (real mode), print "Hello, OS world!" on the screen, load the system image and wait for any keyboard input to switch into protected mode.

> [cli](https://en.wikipedia.org/wiki/Interrupt_flag#CLI) -  close interrupts, used in operating systems to disable interrupts so kernel code (typically a driver) can avoid race conditions with an interrupt handler. Compare [sti](https://en.wikipedia.org/wiki/Interrupt_flag#STI).
>
> The [VGA](https://en.wikipedia.org/wiki/VGA-compatible_text_mode) text buffer is located at physical memory address 0xB8000, we print "Hello, OS world!" in real mode by writing directly to the VGA text buffer.
>
> [BIOS interrupt call 13H](https://en.wikipedia.org/wiki/INT_13H) - reset boot floppy and read sectors from drive in  `bootsect/start16.S`.
>
> [The Global Descriptor Table (GDT)](https://en.wikipedia.org/wiki/Global_Descriptor_Table) -  a table in memory that defines the processor's memory segments. The GDT sets the behavior of the segment registers and helps to ensure that protected mode operates smoothly.

General speaking, the boot loader is a small program that runs before the operating system kernel kicks in. Through this small program, we can initialize the hardware device and establish a map of the memory space.

The [MBR](https://en.wikipedia.org/wiki/Master_boot_record) consists of 512 or more bytes located in the first sector of the drive. A classical generic MBR may contain:

* 1- 446 bytes: Bootstrap code area;
* 447 - 510 bytes: Partition entry 1 - 4;
* 511 - 512 bytes: Boot signature (0x55 and 0xaa).

Read in `bootsect/Makefile`. Read [GNU linker](https://en.wikipedia.org/wiki/GNU_linker) and [ld (Unix)](https://en.wikipedia.org/wiki/Linker_(computing)) to better understand the operations here.

*hint: explain `0x7C00` and `0x7C00 + 508` in `bootsect/start16.ld`*.

For faster execution speed, stage 2 of the boot loader is usually loaded into RAM.

Since stage 2 is usually written in C, in addition to the system image, we should also take stack space into consideration. The size of the space is preferably a multiple of the memory page size (usually 4KB). In general, a 1M RAM space is sufficient.

**Boot loader Stage 2 (Entering C)** 

In `myOS/start32.S`, we set up the stack and jump to the C entry point of `userApp/main.c:myMain()`.

*hint: Why do we need to zero out the BSS segment in  `myOS/start32.S`?*

Read [Data segment structure](https://en.wikipedia.org/wiki/Data_segment) to understand the contents of linker script `myOS/start32.ld`. 

```
/*
 * myOS.ld
 *
 * Linker script for the i386 bootsect code
 */
OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
	. = 0x7E00;
	_text_start = .;
	.text		: { *(.text*) }
	
	. = ALIGN(16);
	.data		: { *(.data*) }
	
	. = ALIGN(16);
	.bss		:
	{
		__bss_start = .;
		_bss_start = .;
		*(.bss)
		__bss_end = .;
	}
	. = ALIGN(16);
	_end = .;
	. = ALIGN(512);	
}
```

*hint: Why does the stage 2 of boot loader start at `0x7E00`?*

You're expected to add assembly code for context switch in `myOS/start32.S`, it should be something like:

```asm
.global CTX_SW
CTX_SW: 
	pushf 
  	pusha
	movl prevTSK_StackPtr,%eax
	movl %esp, (%eax)
	movl nextTSK_StackPtr, %esp
	popa 
	popf
	ret
```

> [pushf](https://www.felixcloutier.com/x86/pushf:pushfd:pushfq) - pushes the flags register onto the top of the stack.
>
> [pusha](http://faydoc.tripod.com/cpu/pusha.htm) - pushes all the general purpose registers onto the stack in the following order: AX, CX, DX, BX, SP, BP, SI, DI.
>
> [popa](https://docs.oracle.com/cd/E19455-01/806-3773/instructionset-114/index.html) - This instruction pops all the general purpose registers off the stack in the reverse order of PUSHA.

...where `prevTSK_StackPtr` is the pointer to the stack of the current task, `nextTSK_StackPtr` is the pointer to the stack of next task in queue. When using this method, declare these pointers in  `myOS/task.c` as `extern` and declare them as `.global` in `myOS/start32.S`.

Or you can use [inline assembly](https://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html#s4) in `myOS/task.c`:

```c
void CTX_SW(unsigned long *prevTSK_StackPtr,unsigned long *nextTSK_StackPtr)
{
	asm
	(
        "pushf\n\t"
		"pusha\n\t"
		"movl %%esp, %0\n\t" 
		"movl %1, %%esp\n\t" 
		"popa\n\t"
        "popf\n\t"
		"ret"
		:
		:"m"(prevTSK_StackPtr),"m"(nextTSK_StackPtr)
	);
}
```

You are expected to implement TCB in `myOS/task.c`, pseudocode:

```c
typedef struct myTCB{
	pid;
	state;
	pointer_to_stack;
	pointer_to_next_TCB; // create a queue for tasks
};
```

Implement  `void osStart(void)` , `int createTsk(void (*tskBody)(void))`, `void tskEnd(void)`, `void initTskBody(void)` in `myOS/task.c`. To initialize stack:

```c
unsigned long* stack;
void stack_init(unsigned long** stk,void (*task)(void)){
  *(*stk)-- = (unsigned long) 0x08;        // CS first 32 bits
  *(*stk)-- = (unsigned long) task;     // eip
  *(*stk)-- = (unsigned long) 0xAAAAAAAA;  // EAX 
  *(*stk)-- = (unsigned long) 0xCCCCCCCC;  // ECX
  *(*stk)-- = (unsigned long) 0xDDDDDDDD;  // EDX 
  *(*stk)-- = (unsigned long) 0xBBBBBBBB;  // EBX
  *(*stk)-- = (unsigned long) 0x44444444;  // ESP
  *(*stk)-- = (unsigned long) 0x55555555;  // EBP
  *(*stk)-- = (unsigned long) 0x66666666;  // ESI
  *(*stk) = (unsigned long) 0x77777777;  // EDI  last 32 bits
    // dummies for debugging
}
```

*hint: When to initialize a task-specific stack?[The pushf assembly instruction](https://www.felixcloutier.com/x86/pushf:pushfd:pushfq) *

To compile this newly added source code, add the following to `myOS/Makefile`: 

```makefile
output/myOS/task.o: myOS/task.c
	gcc -c ${C_FLAGS} myOS/task.c -o output/myOS/task.o
```

Run `./source2img.sh` to run our simple OS on QEMU:

```shell
IMG_NAME=output/a_boot2C.img

echo "Compiling into Binary..."
echo "make clean"
make clean

echo "make"
make

echo "Building boot floppy..."
sudo dd if=output/start16.bin of=${IMG_NAME} bs=512 count=1
sudo dd if=output/myOS.bin of=${IMG_NAME} bs=512 seek=1

echo "qemu-system-i386 -fda ${IMG_NAME}"[The pushf assembly instruction](https://www.felixcloutier.com/x86/pushf:pushfd:pushfq) 
sudo qemu-system-i386 -fda ${IMG_NAME}
```

> The `.img` filename extension is used by disk image files, which contain raw dumps of a magnetic disk or of an optical disc.https://www.ibm.com/developerworks/cn/linux/l-btloader/index.html

*hint: What does `count=1` and `seek=1` imply here?*

*hint: Compare it with Boot with GRUB section in lab 1.* 

### Debugging Tools

**Use hexdump to dump contents of generated object files** 

```shell
hexdump -C output/start16.bin
```

> [Hexdump](https://www.geeksforgeeks.org/hexdump-command-in-linux-with-examples/) is a very useful Linux command for developers and application debuggers. It has the ability to dump file contents into many formats like hexadecimal, octal, ASCII and decimal.

*hint: What does the `55 aa` at the end mean?* 

```shell
hexdump -C output/myOS.bin
hexdump -C output/a_boot2C.img
```

**Use GDB to debug** 

```shell
sudo qemu-system-i386 -fda output/a_boot2C.img -s -S
```

Then, from another local shell:

```shell
gdb
(gdb) file output/myOS.elf
```

...to read [symbol table](https://www.tutorialspoint.com/compiler_design/compiler_design_symbol_table.htm) of our simple OS.

Add breakpoint, connect to QEMU and continue to run kernel:

```shell
(gdb) b myMain
(gdb) target remote:1234
(gdb) c
```

Some GDB commands:

* `r/run` - starts the program running under gdb. Note that every time you enter `r`, your program will restart at the beginning.
* `file` - specifies which program you want to debug.
* `b/break` - sets break points.
* `c/continue` -  sets the program running again, after you have stopped it at a breakpoint.
* `print` - prints out the value of the expression, which could be just a variable name.
* `bt` - prints a backtrace of the entire stack. 
* `i r` - `info registers`
* `watch` - sets watchpoints.

## References

[What is real mode?](https://en.wikipedia.org/wiki/Real_mode)

[What is BIOS interrupt call?](https://en.wikipedia.org/wiki/BIOS_interrupt_call) 

[How does a boot loader work?]([Bootloader](https://en.wikipedia.org/wiki/Booting#First-stage_boot_loader)) 

[What is a linker?](https://www.geeksforgeeks.org/compiler-design-linker/) 

[What is a Task Control Block](https://en.wikipedia.org/wiki/Task_Control_Block)/[Process control block?](https://en.wikipedia.org/wiki/Process_control_block) 

[What is a multitasking OS?](https://en.wikipedia.org/wiki/Computer_multitasking) 

[What is FCFS?](https://www.techopedia.com/definition/23455/first-come-first-served-fcfs) 

[A simple Makefile tutorial](http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/) 

[Learning to Read x86 Assembly Language](http://patshaughnessy.net/2016/11/26/learning-to-read-x86-assembly-language) 

[What is Global Descriptor Table (GDT)?](https://en.wikipedia.org/wiki/Global_Descriptor_Table) 

[VGA-compatible text mode](https://en.wikipedia.org/wiki/VGA-compatible_text_mode) 

[BIOS interrupt call 13H](https://en.wikipedia.org/wiki/INT_13H) 

[The cli assembly instruction](https://en.wikipedia.org/wiki/Interrupt_flag#CLI) 

[The sti assembly instruction](https://en.wikipedia.org/wiki/Interrupt_flag#STI) 

[MBR layout](https://en.wikipedia.org/wiki/Master_boot_record#Sector_layout) 

[GNU linker](https://en.wikipedia.org/wiki/GNU_linker) and [ld (Unix)](https://en.wikipedia.org/wiki/Linker_(computing)) 

[What is stack-based memory allocation](https://en.wikipedia.org/wiki/Stack-based_memory_allocation)

[What is the structure of data segment?](https://en.wikipedia.org/wiki/Data_segment) 

[The pusha assembly instruction](http://faydoc.tripod.com/cpu/pusha.htm) 

[The popa assembly instruction](https://docs.oracle.com/cd/E19455-01/806-3773/instructionset-114/index.html) 

[The pushf assembly instruction](https://www.felixcloutier.com/x86/pushf:pushfd:pushfq) 

[What is context switch?]([Context switch](https://en.wikipedia.org/wiki/Context_switch)) 

[A simple inline assemble tutorial](https://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html#s4) 

[The stack framework - an example](http://alanclements.org/TEST_StackFrames2.pdf) 

[IMG (file format)](https://en.wikipedia.org/wiki/IMG_(file_format)) 

[What is a compiler?](https://en.wikipedia.org/wiki/Compiler) 

[What is & how to makefile?](https://opensource.com/article/18/8/what-how-makefile)  

[Simple linker example](https://sourceware.org/binutils/docs/ld/Simple-Example.html) 

[Writing a Tiny x86 Bootloader](http://joebergeron.io/posts/post_two.html) 

[The hexdump command]([Hexdump](https://www.geeksforgeeks.org/hexdump-command-in-linux-with-examples/)) 

[What is symbol table](https://www.tutorialspoint.com/compiler_design/compiler_design_symbol_table.htm) 
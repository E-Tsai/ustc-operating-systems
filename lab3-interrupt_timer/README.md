# Lab 3: Realize Multiple Scheduling Algorithms

*Realize Shortest Job First/Priority/Round Robin/Multilevel Queue/Multilevel Feedback Queue scheduler based on lab 2.*

## Purpose

In this experiment, we're going to realize a Shortest Job First/Priority/Round Robin/Multilevel Queue scheduler based on lab 2. Participants are expected to:

* Understand the following OS concepts:
  * [Preemptive and Non-Preemptive Scheduling](https://www.geeksforgeeks.org/preemptive-and-non-preemptive-scheduling/);
  * [Shortest Job First](https://en.wikipedia.org/wiki/Shortest_job_next) scheduling;
  * [Priority](https://en.wikipedia.org/wiki/Aging_(scheduling)) scheduling;
  * [Round-Robin](https://en.wikipedia.org/wiki/Round-robin_scheduling) scheduling;
  * [time slice](https://en.wikipedia.org/wiki/Preemption_(computing)#Time_slice);
  * [Multilevel Queue](https://en.wikipedia.org/wiki/Multilevel_queue#Process_Scheduling) scheduling;
  * [Multilevel feedback queue](https://en.wikipedia.org/wiki/Multilevel_feedback_queue) scheduling;
  * [Interrupt descriptor table (IDT)](https://en.wikipedia.org/wiki/Interrupt_descriptor_table);
  * real time and [non real time](https://www.computerhope.com/jargon/n/nrt.htm) tasks;
* Understand provided source code and be able to answer the following questions:
  * What is the difference between `myOS/start32.S` in lab 2 and 3?
  * How to perform BIOS interrupts in protected mode?
  * How is the timer for RR algorithm implemented?

## Contents

* Read the source code provided.
* Implement scheduling algorithms in next section.
* Write test code for each scheduler you implemented.
* Compile source code into binary.
* Build a boot floppy.
* Run with QEMU.

## Requirements

* Modify atomic operations implemented in lab 2 to set parameters for tasks (e.g. priority, execute time, arrive time, scheduling policy).

* Modify `osStart()` to add corresponding functions.

* Realize following scheduling algorithms:

  - level 0 - FCFS scheduling (lab 2).

  - level 1 - SJF scheduling (preemptive or non preemptive).

  - level 1 - RR scheduling.

  - level 1 -  Priority scheduling (preemptive or non preemptive), with no two tasks having the same priority.

  - level 2 -  Priority scheduling (preemptive or non preemptive). Tasks with same priority scheduled with FCFS or RR.

  - level 3 - Two-level queue scheduling. Use priority scheduling to schedule real-time tasks (preemptive or non preemptive). Real time tasks can be set as `SCHED_RT_FCFS` or `SCHED_RT_RR`; Non real time tasks is `SCHED_RR`;
  - Multilevel feedback queue scheduling.

  Participants are required to implement at least one algorithm in level one (3 points). 

* Provide corresponding test cases for each algorithm implemented:

  * Use `setTskPara()`, `getTskPara()` to set parameters of tasks.

‌

## Instructions

### Code Structure

Directory structure:

```
├── bootsect
│   ├── Makefile
│   ├── start16.ld
│   └── start16.S
├── Makefile
├── myOS
│   ├── Makefile
│   ├── myOS.h
│   ├── myOS.ld
│   ├── myPrintf.c
│   ├── mytask_sched.h			# contains info for schedulers
│   ├── start32.S
│   ├── task.c
│   ├── timer.c					# timer for RR
│   └── vsprintf.c
└── userApp
    ├── init.c
    ├── main.c
    ├── Makefile
    └── userApp.h

```

The compiling process:

```
   bootsect/                 myOS/                                      userApp/          
-------------------------------------------------------------------------------------- 
   start16.S         start32.S   task.c*  timer.c   myPrintf.c       init.c    main.c 
       |                 |         |        |          |               |         |
       |                 |         |        |          |               |         | 
   start16.o         start32.o   task.o   timer.o   myPrintf.o       init.o    main.o 
       |                 |         |         |         |               |         |
       | start16.ld      |         |         |         |               |         |
       |                 --------------------|------------------------------------  
  start16.elf                                | myOS.ld        
       |                                  myOS.elf          
       |                                     |                
  start16.bin                             myOS.bin      
       \                                     /              
        \                                   /             
         \             bootloader          /            
        +------------------------|-----------------------+                    
        |          MBR           |          OS           |  a_boot2C.img  
        +------------------------------------------------+             
       0x7C00                   0x7E00             
```



### Walk Through

*With prior knowledge about the booting process of our simple OS, participants are expected to implement several different scheduling policy on their OS.*

We provide BIOS interrupts and timer in `myOS/start32.S`.

> IDT - a data structure used by the x86 architecture to implement an interrupt vector table.
>
> [lidt](http://jbwyatt.com/253/IntelOpCodes.html#lidt) - loads a value from an operand into the Interrupt Descriptor Table (IDT) register.

In [real mode]((https://en.wikipedia.org/wiki/Real_mode)), the IVT (interrupt vector table) always resides at the same location in memory, ranging from `0x0000` to `0x03ff`, and consists of 256 four-byte real mode far pointers (256 × 4 = 1024 bytes of memory). Through simple instruction like `int $0x5` ,  corresponding `CS:IP` stored in the IVT is loaded into `CS ` and `IP`, and the program jumps to the entry of the 5th interrupt-function.

In [protected mode](https://en.wikipedia.org/wiki/Protected_mode), the IDT is an array of 8-byte descriptors stored consecutively in memory and indexed by an interrupt vector. These descriptors may be either interrupt gates, trap gates or task gates. 

The protected mode IDT may reside anywhere in physical memory. The processor has a special register (`IDTR`) to store both the physical base address and the length in bytes of the IDT. When an interrupt occurs, the processor multiplies the interrupt vector by 8 and adds the result to the IDT base address. With help of the IDT length, the resulting memory address is then verified to be within the table; if it is too large, an exception is generated. If everything is okay, the 8-byte descriptor stored at the calculated memory location is loaded and actions are taken according to the descriptor's type and contents.

> [Intel 8259](https://en.wikipedia.org/wiki/Intel_8259) - a Programmable Interrupt Controller (PIC) designed for the Intel 8085 and Intel 8086 microprocessors. 
>
> [Intel 8253](https://en.wikipedia.org/wiki/Intel_8253) -  [Programmable Interval Timers](https://en.wikipedia.org/wiki/Programmable_Interval_Timer) (PITs), which perform timing and counting functions using three 16-bit counters.

We use these modules to realize our RR algorithm.

To enable timer, your `osStart()` function should be something like:

```c
void osStart(void){
	// other init operations
	init8259A();
	tick_number = 0;
	init8253();
	// set scheduler parameters
	enable_interrupt();
    
    //////////////////////////////////////////////
	// print relavant info
	TaskManagerInit();
	
	// init stack pointers 
	
	// context switch
}
```

You are supposed to add information of task parameters, scheduler type, queues for corresponding scheduler into proper header files.

Read `tick_hook()` in `userApp/main.c` and implement RR algorithm.

‌Provide corresponding test cases for each algorithm implemented, explain your test case design. 

Check out the head file references and `userApp` replacement we provided.

## References

[What is Shortest Job First?](https://en.wikipedia.org/wiki/Shortest_job_next) 

[What is Aging (scheduling)?](https://en.wikipedia.org/wiki/Aging_(scheduling)) 

[What is Multilevel Queue?](https://en.wikipedia.org/wiki/Multilevel_queue#Process_Scheduling) 

[What is the time slice?](https://en.wikipedia.org/wiki/Preemption_(computing)#Time_slice) 

[What is the Interrupt descriptor table (IDT)?](https://en.wikipedia.org/wiki/Interrupt_descriptor_table)

[The lidt assembly instruction](http://jbwyatt.com/253/IntelOpCodes.html#lidt) 

[What is intel 8259?](https://en.wikipedia.org/wiki/Intel_8259) 

[What is Intel 8253?](https://en.wikipedia.org/wiki/Intel_8253)

[Preemptive and Non-Preemptive Scheduling](https://www.geeksforgeeks.org/preemptive-and-non-preemptive-scheduling/) 
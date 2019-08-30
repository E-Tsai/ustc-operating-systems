STACK_SIZE =	0x1000	# Size of heap and stack. 4KB
WHITE	= 0x7			# 白色，用于输出字符，为字符颜色
VGA_START	= 0xb8000

/* ===== code32 =========================== */

.text
	.globl 	start		# GNU default entry point
	.globl 	myMain
.code32
start:
its_ok:
	movl	$ok_message, %esi
	movb	$WHITE, %ah
#	movl	$0, %edx 	# %edx is set above
	call	pm_print_string

	jmp establish_stack

dead:	jmp dead			# Never here

# 在调用pm_print_string之前，必须先进行如下赋值
#	movl	string, %esi 
#	movb	color, %ah
#	movl	offset, %edx
pm_print_string:
	movl	$VGA_START, %ebx	
pm_print_string_loop:	
	lodsb
	andb	%al, %al
	jz	pm_print_string_end
	movw	%ax, %es:0(%ebx,%edx,2)
	incl	%edx
	jmp	pm_print_string_loop
pm_print_string_end:
	ret

# Set up the stack
establish_stack:
	movl	$_end, %eax		# eax = end of bss/start of heap
	addl	$STACK_SIZE, %eax	# make room for stack
	andl	$0xffffffc0, %eax	# align it on 16 byte boundary
	
	movl	%eax, %esp		# set stack pointer
	movl	%eax, %ebp		# set base pointer

# Zero out the BSS segment
zero_bss:
	cld				              # make direction flag count up
	movl	$_end, %ecx		    # find end of .bss
	movl	$_bss_start, %edi	# edi = beginning of .bss
	subl	%edi, %ecx		    # ecx = size of .bss in bytes
	shrl	%ecx			        # size of .bss in longs
	shrl	%ecx
	
	xorl	%eax, %eax		    # value to clear out memory
	repne			            	# while ecx != 0
	stosl				            # clear a long in the bss


setup_idtptr:
	call setup_idt
	lidt idtptr

# Transfer control to main
to_main:
	movl	$to_main_message, %esi 
	movb	$WHITE, %ah
	movl	$(11*80), %edx 	
	call	pm_print_string
	
	call	myMain

shut_down:	
	jmp	shut_down	# Never here
	
# 上下文切换
# 上下文示意图：
# （高地址）CS：IP
# （低地址）
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

	.p2align 4
time_interrupt:	
	cld
	//pushf
  	pusha
	call tick
	popa
	//popf
	iret

	.p2align 4
ignore_int1:
	cld
	pusha
	movl	$int_message1, %esi 
	movb	$WHITE, %ah
	movl	$(24*80), %edx 	
	call	pm_print_string
	popa
	iret
	
setup_idt:
	movl $ignore_int1,%edx
	movl $0x00080000,%eax
	movw %dx,%ax		/* selector = 0x0010 = cs */
	movw $0x8E00,%dx	/* interrupt gate - dpl=0, present */

	movl $IDT,%edi
	mov $256,%ecx
rp_sidt:
	movl %eax,(%edi)
	movl %edx,4(%edi)
	addl $8,%edi
	dec %ecx
	jne rp_sidt

    # ret  /* if do not set timer*/

setup_time_int_32:	
	movl $time_interrupt,%edx
	movl $0x00080000,%eax
	movw %dx,%ax		/* selector = 0x0010 = cs */
	movw $0x8E00,%dx	/* interrupt gate - dpl=0, present */

	movl $IDT,%edi
	addl $(32*8), %edi
	movl %eax,(%edi)
	movl %edx,4(%edi)

	ret

	.globl init8259A
init8259A:
	mov $0xff, %al
	out %al, $0x21
	out %al, $0xA1
	
	mov $0x11, %al
	out %al, $0x20
	
	mov $0x20, %al
	out %al, $0x21
	
	mov $0x04, %al
	out %al, $0x21
#define AUTO_EOI 1
#if AUTO_EOI
	mov $0x03, %al
	out %al, $0x21
#else
  mov $0x01, %al
	out %al, $0x21
#endif
  	
  mov $0x11, %al
	out %al, $0xA0
	
	mov $0x28, %al
	out %al, $0xA1
	
	mov $0x02, %al
	out %al, $0xA1

  mov $0x01, %al
	out %al, $0xA1

	ret

	.globl enable_interrupt
enable_interrupt:
	sti
	ret

	.globl disable_interrupt
disable_interrupt:
	cli
	ret
	
	.globl init8253
init8253:
	mov $0x34, %al
	out %al, $0x43
	
  # ?100HZ?
	mov $(11932 & 0xff), %al
	out %al, $0x40
	
	mov $(11932 >> 8), %al
	out	%al, $0x40	
	
	in $0x21, %al 
	andb $0xFE, %al
	out %al, $0x21
	ret

/* ===== data =========================== */
.data
		.globl _stack_size
_stack_size:	.long STACK_SIZE

ok_message:
	.ascii	"O.K.!"
	.byte	0

to_main_message:
	.ascii	"Call myMain..."
	.byte	0

int_message1:
	.ascii "Unknown interrupt1"
	.byte 0	

# IDT
	.p2align 4
	.globl IDT
IDT:
	.rept 256
	.word 0,0,0,0
	.endr

idtptr:
	.word  (256*8 - 1)
	.long  IDT	

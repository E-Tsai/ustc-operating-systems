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

# Transfer control to main
to_main:
	movl	$to_main_message, %esi 
	movb	$WHITE, %ah
	movl	$(11*80), %edx 	
	call	pm_print_string
	
	call	myMain

shut_down:	
	jmp	shut_down	# Never here
	
#上下文切换
#上下文示意图：
#（高地址）CS：IP
#（低地址）
  .global CTX_SW
CTX_SW:  
  pusha #pushl %ebp
	movl %esp, prevTSK_StackPtr
	
	movl nextTSK_StackPtr, %esp
	popa #popl %ebp
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

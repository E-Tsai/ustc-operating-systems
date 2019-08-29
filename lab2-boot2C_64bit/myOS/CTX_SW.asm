AREA    EXAMPLE,CODE,READONLY
EXPORT CTX_SW
ENTRY
/*context switch*/
CTX_SW
	pusha 
	movl %esp, r0 
	movl r1, %esp 
	popa 
	ret
	END 
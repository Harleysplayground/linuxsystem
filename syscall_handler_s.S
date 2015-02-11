.globl syscall_handler
syscall_handler:
	pushal             #push EAX,EBX,ECX,EDX,original ESP,EBP,ESI,EDI
	#push CS DS ES FS GS SS
	pushl %edx
	pushl %ecx
	pushl %ebx
	addl $-1, %eax
	call *operations(,%eax,4)
	addl $12, %esp
	popal              #push EAX,EBX,ECX,EDX,ESP,EBP,ESI,EDI
	iret

operations:
	.long halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn


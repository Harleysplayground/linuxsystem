.globl excp0_handler
excp0_handler:
    CLI
	pushal             #push EAX,EBX,ECX,EDX,ESP,EBP,ESI,EDI
	push $0
	call excp_handler
	add $4, %esp
	popal              #push EAX,EBX,ECX,EDX,ESP,EBP,ESI,EDI
	iret

.globl excp1_handler
excp1_handler:
    CLI
	pushal
	push $1
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp2_handler
excp2_handler:
    CLI
	pushal
	push $2
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp3_handler
excp3_handler:
    CLI
	pushal
	push $3
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp4_handler
excp4_handler:
    CLI
	pushal
	push $4
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp5_handler
excp5_handler:
    CLI
	pushal
	push $5
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp6_handler
excp6_handler:
    CLI
	pushal
	push $6
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp7_handler
excp7_handler:
    CLI
	pushal
	push $7
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp8_handler
excp8_handler:
    CLI
	pushal
	push $8
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp9_handler
excp9_handler:
    CLI
	pushal
	push $9
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp10_handler
excp10_handler:
    CLI
	pushal
	push $10
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp11_handler
excp11_handler:
    CLI
	pushal
	push $11
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp12_handler
excp12_handler:
    CLI
	pushal
	push $12
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp13_handler
excp13_handler:
    CLI
	pushal
	movl 32(%esp), %eax
	pushl %eax
	movl 40(%esp), %eax
	pushl %eax
	movl 48(%esp), %eax
	pushl %eax
	push $13
	call excp_handler
	add $16, %esp
	popal
	iret

.globl excp14_handler
excp14_handler:
    CLI
	pushal
	movl 32(%esp), %eax
	pushl %eax
	movl 40(%esp), %eax
	pushl %eax
	movl %cr2, %eax
	pushl %eax
	push $14
	call excp_handler
	add $16, %esp
	popal
	iret

.globl excp15_handler
excp15_handler:
    CLI
	pushal
	push $15
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp16_handler
excp16_handler:
    CLI
	pushal
	push $16
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp17_handler
excp17_handler:
    CLI
	pushal
	push $17
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp18_handler
excp18_handler:
    CLI
	pushal
	push $18
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp19_handler
excp19_handler:
    CLI
	pushal
	push $19
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp20_handler
excp20_handler:
    CLI
	pushal
	push $20
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp21_handler
excp21_handler:
    CLI
	pushal
	push $21
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp22_handler
excp22_handler:
    CLI
	pushal
	push $22
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp23_handler
excp23_handler:
    CLI
	pushal
	push $23
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp24_handler
excp24_handler:
    CLI
	pushal
	push $24
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp25_handler
excp25_handler:
    CLI
	pushal
	push $25
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp26_handler
excp26_handler:
    CLI
	pushal
	push $26
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp27_handler
excp27_handler:
    CLI
	pushal
	push $27
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp28_handler
excp28_handler:
    CLI
	pushal
	push $28
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp29_handler
excp29_handler:
    CLI
	pushal
	push $29
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp30_handler
excp30_handler:
    CLI
	pushal
	push $30
	call excp_handler
	add $4, %esp
	popal
	iret

.globl excp31_handler
excp31_handler:
    CLI
	pushal
	push $31
	call excp_handler
	add $4, %esp
	popal
	iret


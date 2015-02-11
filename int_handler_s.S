.globl irq0_handler
irq0_handler:
    CLI
	pushal             #push EAX,EBX,ECX,EDX,ESP,EBP,ESI,EDI
	pushl $0
	call int_handler
	addl $4, %esp
	popal              #push EAX,EBX,ECX,EDX,ESP,EBP,ESI,EDI
	iret

.globl irq1_handler
irq1_handler:
    CLI
	pushal
	pushl $1
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq2_handler
irq2_handler:
    CLI
	pushal
	pushl $2
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq3_handler
irq3_handler:
    CLI
	pushal
	pushl $3
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq4_handler
irq4_handler:
    CLI
	pushal
	pushl $4
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq5_handler
irq5_handler:
    CLI
	pushal
	pushl $5
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq6_handler
irq6_handler:
    CLI
	pushal
	pushl $6
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq7_handler
irq7_handler:
    CLI
	pushal
	pushl $7
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq8_handler
irq8_handler:
    CLI
	pushal
	pushl $8
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq9_handler
irq9_handler:
    CLI
	pushal
	pushl $9
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq10_handler
irq10_handler:
    CLI
	pushal
	pushl $10
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq11_handler
irq11_handler:
    CLI
	pushal
	pushl $11
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq12_handler
irq12_handler:
    CLI
	pushal
	pushl $12
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq13_handler
irq13_handler:
    CLI
	pushal
	pushl $13
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq14_handler
irq14_handler:
    CLI
	pushal
	pushl $14
	call int_handler
	addl $4, %esp
	popal
	iret

.globl irq15_handler
irq15_handler:
    CLI
	pushal
	pushl $15
	call int_handler
	addl $4, %esp
	popal
	iret

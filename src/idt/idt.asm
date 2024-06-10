section .asm

extern int21h_handler
extern no_interrupt_handler

global idt_load
global int21h
global no_interrupt

idt_load:
	push ebp
	mov ebp, esp
	
	mov ebx, [ebp+8] ; ebp points to base, ebp+4 points to return stack of caller, ebp+8 points to first arg

	lidt[ebx]  ; load IDT

	pop ebp
	ret

int21h:
	cli
	pushad ; pushes all General Purpose registers
	call int21h_handler
	popad ; pops all GP registers
	sti

	iret ; interrupt return

no_interrupt:
	cli
	pushad ; pushes all General Purpose registers
	call no_interrupt_handler
	popad ; pops all GP registers
	sti

	iret ; interrupt return
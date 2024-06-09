section .asm

global idt_load

idt_load:
	push ebp
	mov ebp, esp
	
	mov ebx, [ebp+8] ; ebp points to base, ebp+4 points to return stack of caller, ebp+8 points to first arg

	lidt[ebx]  ; load IDT

	pop ebp
	ret
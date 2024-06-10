section .asm

global insb
global insw
global outb
global outw


insb:
	push ebp
	mov ebp, esp
 
	xor eax, eax  ; make eax zero
	mov edx, [ebp+8]  ; set edx to first parameter of C function which is port
	in al, dx  ; eax least significant byte is set

	pop ebp
	ret

insw:
	push ebp
	mov ebp, esp
 
	xor eax, eax  ; make eax zero
	mov edx, [ebp+8]  ; set edx to first parameter of C function which is port
	in ax, dx  ; eax least significant word is set

	pop ebp
	ret

outb:
	push ebp
	mov ebp, esp
 
	mov eax, [ebp+12]  ; set eax to second parameter of C function which is val
	mov edx, [ebp+8]  ; set edx to first parameter of C function which is port
	out dx, al  ; eax least significant byte is used

	pop ebp
	ret

outw:
	push ebp
	mov ebp, esp
 
	mov eax, [ebp+12]  ; set eax to second parameter of C function which is val
	mov edx, [ebp+8]  ; set edx to first parameter of C function which is port
	out dx, ax  ; eax least significant word is used

	pop ebp
	ret
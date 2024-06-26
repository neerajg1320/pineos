[BITS 32]

section .asm

global paging_load_directory
global enable_paging

paging_load_directory:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]
    mov cr3, eax  ; cr3 should contain address of our page directory
    pop ebp
    ret

enable_paging:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or  eax, 0x80000000
    mov cr0, eax ; set the bit 31 on cr0
    pop ebp
    ret
        
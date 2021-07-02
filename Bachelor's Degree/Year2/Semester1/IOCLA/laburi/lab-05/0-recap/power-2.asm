%include "../utils/printf32.asm"

section .text
extern printf
global main
main:
    push ebp
    mov ebp, esp

    mov eax, 211    ; to be broken down into powers of 2
    mov ebx, 1      ; stores the current power

    ; TODO - print the powers of 2 that generate number stored in EA
    mov ecx, 32

label:
	test eax, ebx
	jnz print
label2:
	shl ebx, 1
	jnz label

leave
ret

print:
    PRINTF32 `%d\n\x0`, ebx
	sub eax, ebx
    jmp label2

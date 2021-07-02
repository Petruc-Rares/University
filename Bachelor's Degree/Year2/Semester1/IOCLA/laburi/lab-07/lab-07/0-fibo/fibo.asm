%include "../utils/printf32.asm"

%define NUM_FIBO 7

section .text

extern printf
global main
main:
    mov ebp, esp

    ; TODO - replace below instruction with the algorithm for the Fibonacci sequence
    push 0
    push 1

    mov ecx, NUM_FIBO
    mov edx, NUM_FIBO
    sub edx, 2

umple_stiva:
    pop eax
    pop ebx
    push ebx
    push eax
    add eax, ebx
    push eax
    sub edx, 1
    jnz umple_stiva


print:
    mov eax, dword [esp + (ecx - 1) * 4]
    PRINTF32 `%d \x0`, eax
    dec ecx
    cmp ecx, 0
    ja print

    PRINTF32 `\n\x0`
    mov esp, ebp
    xor eax, eax
    ret

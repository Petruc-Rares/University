%include "io.mac"

section .data
    itHay dd 0
    itNeedle dd 0
    sizeNeedle dd 0

section .text
    global my_strstr
    extern printf

my_strstr:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edi, [ebp + 8]      ; substr_index
    mov     esi, [ebp + 12]     ; haystack
    mov     ebx, [ebp + 16]     ; needle
    mov     ecx, [ebp + 20]     ; haystack_len
    mov     edx, [ebp + 24]     ; needle_len
    ;; DO NOT MODIFY

    mov dword [itHay], -1
    mov dword [itNeedle], 0
    mov dword [sizeNeedle], edx


    mov     edx, edi      ; substr_index

searchNeedle:
    add dword [itHay], 1

    mov dword edi, [itHay]
    mov byte al, [esi + edi]

    mov dword edi, [itNeedle]
    mov byte ah, [ebx + edi]

    ; if (hasystack[itHay] == needle[itNeedle])
    xor al, ah
    jz  incrementItNeedle
    ; else
    mov dword [itNeedle], 0

testFound:
    ; test if j == neddle_len
    mov eax, [itNeedle]
    mov edi, [sizeNeedle]
    xor eax, edi
    jz savePosition
    ; else search more

    loop searchNeedle

    ; daca se ajunge aici nu s-a gasit acul
    mov edi, [itHay]
    add edi, 2          ; add 1 to reach haylength
                        ; add another 1 to fulfill the task
    mov [edx], edi 


endProgram:
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY

incrementItNeedle:
    add dword [itNeedle], 1
    jmp testFound

savePosition:
    mov edi, [itHay]
    sub edi, [itNeedle]
    add edi , 1

    mov [edx], edi
    jmp endProgram
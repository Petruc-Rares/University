%include "io.mac"

section .text
    global caesar
    extern printf

caesar:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; ciphertext
    mov     esi, [ebp + 12]     ; plaintext
    mov     edi, [ebp + 16]     ; key
    mov     ecx, [ebp + 20]     ; length
    ;; DO NOT MODIFY

checkUppercase:
    xor eax, eax
    mov al, [esi + ecx - 1]     ; al = plaintext[ecx-1]
    cmp eax, 65
    jl checkNext
    cmp eax, 90
    jg checkLowercase

    ; if reach here 'A' < al < 'Z'
    mov bl, 26          ; get the %26 in here
    sub al, 65          ; current number - 'A'
    add eax, edi        ; current number + index
    div bl              ; make the divison
    mov al, ah;         ; move the remainder in bl
    add al, 65          ; add 'A' to it
    jmp checkNext

checkLowercase:
    cmp al, 97
    jl checkNext
    cmp al, 122
    jg checkNext

    ; if reach here 'a' < al < 'z'
    mov bl, 26          ; get the %26 in here
    sub al, 97          ; current number - 'a'
    add eax, edi        ; current number + index
    div bl              ; make the divison
    mov al, ah;         ; move the remainder in bl
    add al, 97          ; add 'a' to it


checkNext:
    mov byte [edx + ecx - 1], al
    loop checkUppercase

    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY
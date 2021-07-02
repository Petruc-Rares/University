%include "io.mac"

section .text
    global otp
    extern printf

otp:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; ciphertext
    mov     esi, [ebp + 12]     ; plaintext
    mov     edi, [ebp + 16]     ; key
    mov     ecx, [ebp + 20]     ; length
    ;; DO NOT MODIFY

    ;; TODO: Implement the One Time Pad cipher

label:
	mov al, [esi + ecx - 1]		; al = plaintext[ecx-1]
	mov bl, [edi + ecx - 1]		; bl = key[ecx-1]
	xor al, bl					; al = al^bl
	mov byte [edx + ecx - 1], al; cyphertext[ecx - 1] = al

	loop label
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY
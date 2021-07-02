%include "io.mac"

section .data
	itPlain dd 0
	itKey dd 0
	plaintext_len dd 0
	key_len dd 0
	aux dd 0

section .text
    global vigenere
    extern printf

vigenere:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; ciphertext
    mov     esi, [ebp + 12]     ; plaintext
    mov     ecx, [ebp + 16]     ; plaintext_len
    mov     edi, [ebp + 20]     ; key
    mov     ebx, [ebp + 24]     ; key_len
    ;; DO NOT MODIFY

    mov dword [itPlain], 0
    mov dword [itKey], 0
    mov dword [plaintext_len], ecx
    mov dword [key_len], ebx

checkUppercase:
    xor eax, eax

    mov ecx, [itPlain]
	mov al, [esi + ecx]		; al = plaintext[ecx-1]
	cmp eax, 65
	jl checkNext
	cmp eax, 90
	jg checkLowercase
	; if reach here 'A' < al < 'Z'
	cmp [itKey], ebx
	jge resetItKey

newAl_uppercase:
	mov ecx, [itKey]		
	mov al, [edi + ecx]			; sa stim ce corespunde din key
	sub al, 65					; cu cat adunam
	
	mov [aux], al 				; avem in aux ce trb sa adunam

	mov ecx, [itPlain]
	mov al, [esi + ecx]

	sub al, 65 			; current number - 'a'
	add eax, [aux] 		; current number + index
	
	mov cl, 26;
	div cl 			 	; make the divison
	mov al, ah; 		; move the remainder in bl
	add al, 65	 		; add 'a' to it

	inc dword [itKey]
	jmp checkNext

checkLowercase:
	cmp al, 97
	jl checkNext
	cmp al, 122
	jg checkNext
	; if reach here 'a' < al < 'z'
	cmp [itKey], ebx
	jge resetItKey_2

newAl_lowercase:
	mov ecx, [itKey]
	mov al, [edi + ecx]
	sub al, 65

	mov [aux], al 				; avem in aux ce trb sa adunam

	mov ecx, [itPlain]
	mov al, [esi + ecx]

	sub al, 97 			; current number - 'a'
	add eax, [aux] 		; current number + index
	
	mov cl, 26;
	div cl 			 	; make the divison
	mov al, ah; 		; move the remainder in bl
	add al, 97	 		; add 'a' to it

	inc dword [itKey]


	jmp checkNext


checkNext:
	mov ecx, [itPlain]
	mov byte [edx + ecx], al	; save character
	
	inc dword [itPlain]			; itPlain++
	cmp ecx,  [plaintext_len]	; itPlain < plaintext_len
	jl checkUppercase

    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY

resetItKey:
	mov dword [itKey], 0
	jmp  newAl_uppercase

resetItKey_2:
	mov dword [itKey], 0
	jmp  newAl_lowercase
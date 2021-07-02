%include "io.mac"

section .data
    one db 49

section .text
    global bin_to_hex
    extern printf

bin_to_hex:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; hexa_value
    mov     esi, [ebp + 12]     ; bin_sequence
    mov     ecx, [ebp + 16]     ; length
    ;; DO NOT MODIFY

    ;; TODO: Implement bin to hex
  
    mov edi, edx 

    ; vreau sa aflu in cate grupuri
    ; de 4 biti se imparte length
    mov edx, ecx
    shr edx, 16     ; in dx e ok
    mov eax, ecx    ; in ax e ok
    mov ebx, 4
    div bx

    mov bx, ax      ; in bx e catul
    shl ebx, 16     ; vreau ca ebx sa fie de forma 0bx
    shr ebx, 16

    mov edx, edi
    mov edi, ebx     ; mai facem o copie a catului


    ; in bx e catul, il inmultim cu 4
    ; daca este egal cu ecx, inseamna
    ; ca se imparte perfect, deci scad 1 din edi si ebx pentru a ma adapta
    ; la codul scris pentru un length ce
    ; nu divide 4
    shl ebx, 2
    cmp ecx, ebx
    je decrementEdiEbx

setEbxRight:
    shr ebx, 2

    ; daca avem mai putin de un byte
    ; nu se mai intra in whileGroups
    cmp ecx, 4
    jl checkRest


    mov ah, byte [one]; ah = one
    ; cazul cand nu e multiplu de 4
whileGroups:
    mov al, 0

    ; check 4th digit
    cmp byte [esi + ecx - 1], ah
    je setBit4
    
check3rdDigit:
    ; check 3rd digit
    cmp byte [esi + ecx - 2], ah
    je setBit3

check2ndDigit:
    ; check 2nd digit
    cmp byte [esi + ecx - 3], ah
    je setBit2

check1stDigit:
    ; check 1st digit
    cmp byte [esi + ecx - 4], ah
    je setBit1

convertAlHexa:
    cmp al, 10
    jl setDigit
    ; else
    jmp setAlLetter

writeAlEdx:
    mov [edx + ebx], al

    ; get next section of bytes
    sub ebx, 1

    sub ecx, 4
    cmp ecx, 3
    jg whileGroups

    ; acum mai ramane de vazut cate elemente au ramas
    ; dupa ce am impartit unde am putut in grupuri de 4
checkRest:
    mov al, 0

    cmp ecx, 3
    je bitiRamasi3

    cmp ecx, 2
    je bitiRamasi2

    cmp ecx, 1
    je bitiRamasi1

inapoi:
    mov byte [edx + edi + 1], 10    ; line feed


    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY


; ifs 
decrementEdiEbx:
    sub edi, 1
    sub ebx, 1
    jmp setEbxRight

setBit4:
    add al, 1
    jmp check3rdDigit


setBit3:
    add al, 2
    jmp check2ndDigit


setBit2:
    add al, 4
    jmp check1stDigit

setBit1:
    add al, 8
    jmp convertAlHexa

setDigit:
    add al, 48
    jmp writeAlEdx

setAlLetter:
    add al, 55
    jmp writeAlEdx


; structuri ajutatoare pentru 3 biti ramasi
bitiRamasi3:
    ; check 3Rd bit
    cmp byte [esi + ecx - 1], ah
    jmp setBit3_3

    ; check 2nd bit
check2ndBit_3:
    cmp byte [esi + ecx - 2], ah
    je setBit2_3

check1stBit_3:
    ;check 1st bit
    cmp byte [esi + ecx - 3], ah
    je setBit1_3

convertAlHexa_3:
    cmp al, 10
    jl setDigit_3
    ; else
    jmp setAlLetter_3

writeAlEdx_3:
    mov byte [edx + 0], al;
    jmp inapoi


setBit3_3:
    inc al
    jmp check2ndBit_3

setBit2_3:
    add al, 2
    jmp check1stBit_3

setBit1_3:
    add al, 4
    jmp convertAlHexa_3

setDigit_3:
    add al, 48
    jmp writeAlEdx_3

setAlLetter_3:
    add al, 55
    jmp writeAlEdx_3
; asta a fost pentru 3 biti ramasi


; structuri ajutatoare pentru 2 biti ramasi
bitiRamasi2:

    ; check 2nd bit
    cmp byte [esi + ecx - 1], ah
    jmp setBit2_2

    ; check 1st bit
check1stBit_2:
    cmp byte [esi + ecx - 2], ah
    je setBit1_2

convertAlHexa_2:
    cmp al, 10
    jl setDigit_2
    ; else
    jmp setAlLetter_2

writeAlEdx_2:
    mov byte [edx + 0], al;

    jmp inapoi

setBit2_2:
    add al, 2
    jmp check1stBit_2

setBit1_2:
    inc al
    jmp convertAlHexa_2

setDigit_2:
    add al, 48
    jmp writeAlEdx_2

setAlLetter_2:
    add al, 55
    jmp writeAlEdx_2

; asta a fost pentru 2 biti ramasi


; structuri ajutatoare pentru 1 bit ramas

bitiRamasi1:
    cmp byte [esi + ecx - 1], ah
    je setBit1_1

convertAlHexa_1:
    cmp al, 10
    jl setDigit_2
    ; else
    jmp setAlLetter_2

writeAlEdx_1:
    mov byte [edx + 0], al;
    jmp inapoi

setBit1_1:
    inc al
    jmp convertAlHexa_1

setDigit_1:
    add al, 48
    jmp writeAlEdx_1

setAlLetter_1:
    add al, 55
    jmp writeAlEdx_1
; asta a fost pentru 1 bit ramas
section .data
    delim db " ", 0
    stackSize dd 0
    zeroAscii db 48
    minusAscii db 45
    flagMinus db 0
    nullTerminator db 0
    null dd 0

section .bss
    root resd 1

section .text

extern check_atoi
extern print_tree_inorder
extern print_tree_preorder
extern evaluate_tree
extern strtok
extern malloc
extern strdup

global create_tree
global iocla_atoi

createNewNode:
	push ebp
	mov ebp, esp

	push esi
	push edi

	; copy the current pointer
	; otherwise, might be overwritten
	; from strtok in create_tree
	push dword [ebp + 8]
	call strdup
	add esp, 4

	; copy eax new pointer to a string in esi
	mov esi, eax

	mov edi, 12
	call malloc

	; symbol in root
	mov DWORD [eax], esi

	mov ecx, [null]

	; left son
	mov [eax + 4], ecx
	; right son
	mov [eax + 8], ecx

	pop edi
	pop esi

	leave
	ret

iocla_atoi:
	push ebp
	mov ebp, esp

	; make hypothesis that number is positive
	mov byte [flagMinus], 0

	push ebx

	xor eax, eax		; return value (in eax we build the number)
	xor ecx, ecx		; ecx represents an iterator

	mov ebx, [ebp + 8]	; in ebx we have the pointer to the string

	mov edx, [ebp + 8]
	mov dl, [edx + 0]	; get first character in the string
	cmp dl, [minusAscii]
	jne	iterate_String	; if number is positive

negative_number:
	mov byte [flagMinus], 1
	inc ecx


iterate_String:
	; eax *= 10
	mov edx, 10
	mul edx

	; make sure just dl will be populated
	; in edx we have the current letter
	xor edx, edx
	mov dl, [ebx + ecx]
	sub dl, [zeroAscii]

	; eax += current digit 
	add eax, edx

	inc ecx
	cmp byte [ebx + ecx], 0
	jne iterate_String

	; if minus is not needed, no further
	; operations must be done
	cmp byte [flagMinus], 1
	jne leave_iocla_atoi

	; otherwise multiply the number by 1
	mov dword edx, -1
	mul edx

leave_iocla_atoi:

	pop ebx

	leave
	ret


create_tree:
	push ebp
	mov ebp, esp

	; save the calee-saved registers
	; will be popped at the end of this function
    push ebx

    ; store in ebx the pointer to the string
    mov ebx, dword [ebp + 8]


    ; get the first operand in the sequence
	push delim
	push ebx
	call strtok
	add esp, 8

getCurrentSymbol:
	mov dl, [eax + 0] 
	cmp dl, [zeroAscii]
	jl maybeOperand
	jge number

	; an operand has '\0' after it,
	; caused by strtok
maybeOperand:
	mov dl, [eax + 1]
	cmp dl, [nullTerminator]
	je sureOperand
	jne number

sureOperand:
	push eax
	call createNewNode
	add esp, 4

	; push the operand node in stack
	; in order to save it for subsequent
	; operations
	push eax
	add dword [stackSize], 1

	jmp findNextSymbol

number:
	push eax
	call createNewNode
	add esp, 4

	; get the operand, which will be
	; the father of the current number
	pop edx
	sub dword [stackSize], 1

	; check if the operand doesn't have
	; a left son
	mov ecx, [null]
	cmp dword [edx + 4], ecx
	jne add_right


	; reach here if operand has left son null
add_left:
	mov [edx + 4], eax

	push edx
	add dword [stackSize], 1
	jmp findNextSymbol

	; reach here if operand has right son null
add_right:
	mov [edx + 8], eax	

	; program might need to stop
	; if no more elements must be parsed
	cmp dword [stackSize], 0
	je done_from_add_right

	; if reach here, current node
	; needs a father, otherwise current node
	; will need more children available 
	; (impossible, since we work with a binary tree)

get_father:
	; get operand (father of current operand)
	pop ebx			
	sub dword [stackSize], 1

	mov ecx, [null]
	cmp dword [ebx + 4], ecx
	
	je father_left_son_null 

father_right_son_null :
	mov [ebx + 8], edx


	; program might be over
	; by now
	cmp dword [stackSize], 0
	je done_from_father_right_son_null

	; now check which child of the father operand
	; is null and iterate
	mov edx, ebx
	jmp get_father

	jmp findNextSymbol

father_left_son_null :
	mov [ebx + 4], edx
	push ebx
	add dword [stackSize], 1

findNextSymbol:
	push delim
	push dword [nullTerminator]
	call strtok
	add esp, 8

	; check if last occurence
	; of a symbol / number took place
	cmp eax, 0
	jne getCurrentSymbol

done_from_add_right:
	push edx
	jmp done

done_from_father_right_son_null:
	push ebx

done:
	pop eax

end:
	pop ebx

    leave
    ret

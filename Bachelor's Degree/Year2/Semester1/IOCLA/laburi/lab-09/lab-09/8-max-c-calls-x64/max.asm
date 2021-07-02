section .text

extern printf

global get_max

get_max:
	; rdi is pointer to the array
	; rsi is the length of array

	xor rax, rax
	mov rcx, rsi

compare:
	cmp rax, [rdi+rcx*8-8]
	jge check_end
	mov rax, [rdi+rcx*8-8]
check_end:
	loopnz compare



	leave
	ret

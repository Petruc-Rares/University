#include <stdio.h>

#define NUM	0x12345678

int main(void)
{
	size_t n = NUM;
	size_t rot_left;
	size_t rot_right;



	__asm__ (
	"mov ecx, %1\n"		/* store n in ecx */
	"rol ecx, 8\n"		/* rotate left by 8 bits */
	"mov %0, ecx\n\t"
	: "=r" (rot_left)
	: "r" (n)
	: "ecx" );

	__asm__ (
	"mov ecx, %1\n"		/* store n in ecx */
	"ror ecx, 8\n"		/* rotate right by 8 bits */
	"mov %0, ecx\n\t"
	: "=r" (rot_right)
	: "r" (n)
	: "ecx" );

	printf("init: 0x%08x, rot_left: 0x%08x, rot_right: 0x%08x\n",
			n, rot_left, rot_right);

	return 0;
}

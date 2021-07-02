#include <stdio.h>

int main(void)
{
	char cpuid_str[13];

	__asm__ (
	"xor eax, eax\n"
	"cpuid\n"
	"mov esi, %0\n"
	"mov [esi], ebx\n"
	"mov [esi + 4], edx\n"
	"mov [esi + 8], ecx\n"
	:
	: "r" (cpuid_str) /* input variable */
	: "eax", "ebx", "edx", "ecx", "esi" );

	/* TODO: Make cpuid call and copy string in cpuid_str.
	 * eax needs to be 0
	 * After cpuid call string is placed in (ebx, edx, ecx).
	 */

	cpuid_str[12] = '\0';

	printf("CPUID string: %s\n", cpuid_str);

	return 0;
}

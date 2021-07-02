#include<stdio.h>
int main()
{
	int a, *b, *c;
	b = &a;
	c = b;
	a = 4;
	b++;
	printf("%d %d %d", a, *b, *c);	
}

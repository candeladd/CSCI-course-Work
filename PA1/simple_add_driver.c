#include <sys/syscall.h>
#include <stdio.h>
#include <linux/kernel.h>
#include <stdlib.h>
int main()
{
	int *result = malloc(sizeof(result));
	syscall(321, 6, 12, result);
	printf( "result = %d\n", *result);
	free(result);
	return 0;
}


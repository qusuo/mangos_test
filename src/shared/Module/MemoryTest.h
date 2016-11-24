#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void testmemory()
{
	printf("***********************\n");

	time_t timep_start, timep_end;
	time(&timep_start);

	for (int j = 0; j < 1024; ++j)
	{
		for (unsigned int i = 0; i < 1024; ++i)
		{
			char *p = (char*)malloc(i);
			free(p);
		}
	}

	time(&timep_end);
	printf("100wMemoryTest:%d s\n", timep_end - timep_start);
}

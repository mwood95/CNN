#include <stdio.h>
#include <string.h>

void print_matrix(int ** ptr, int len);



int main(void)
{
	int len;
	int i;
	int j;

	scanf("%d", &len);
	
	int *ptr[len];

	// Typecast a malloc call as a pointer to the address that will store
	// len # of int-sized variables. The value of the base address of 
	// memory allocated will be stored in ptr.
	//
	for(i = 0; i < len; i++)
	{
		if((ptr[i] = (int *)malloc(len*sizeof(int))) == NULL)
		{
			printf("Malloc Failed...");
		}
	}

	// Populate the structure	
	for(i = 0; i < len; i++)
	{
		for(j = 0; j < len; j++)
		{	if(i == j)
			{
				(ptr)[i][j] = 1;
			}
			else
			{
				(ptr)[i][j] = 0;
			}
		}
	}

	// Pass the pointer to the base address of the structure we want to use
	printf("Length %d\n", len);
	print_matrix(ptr, len);
	for(i = 0; i < len; i++)
	{
		free(ptr[i]);
	}
	return 0;
}

void print_matrix(int** ptr, int len)
{
	int i = 0;
	int j = 0;
	printf("Length %d\n", len);
	for(i = 0; i < len; i++)
	{
		for(j = 0; j < len; j++)
		{
			printf("%d	", (ptr)[i][j]);
		}
		printf("\n\n");
		
	}
}


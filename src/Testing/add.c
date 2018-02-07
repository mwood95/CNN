#include <stdio.h>
#include <string.h>
void add(float (*ptra)[10][10], float (*ptrb)[10][10], float (*ptrc)[10][10]);

int main()
{
	typedef mytype[10][10];

	float vectora[10][10];
	float vectorb[10][10];
	float vectorc[10][10];

	int i = 0;
	int j = 0;

	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 10; j++)
		{
			if(i == j)
			{
				vectora[i][j] = 1;
				vectorb[i][j] = 1;

			}
			else
			{
				vectora[i][j] = 0;
				vectorb[i][j] = 0;

			}
		}
	}
	mytype *ptra;
	mytype *ptrb;
	mytype *ptrc;


	ptra = &vectora;
	ptrb = &vectorb;
	ptrc = &vectorc;

	add(ptra, ptrb, ptrc);

	printf("\n\n 	Vector A \n\n");

	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 10; j++)
		{
			printf("%f	", vectora[i][j]);
		}
		printf("\n\n");
	}

	printf("\n\n 	Vector B \n\n");
	
	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 10; j++)
		{
			printf("%f	", vectorb[i][j]);
		}
		printf("\n\n");
	}

	printf("\n\n 	Vector C \n\n");

	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 10; j++)
		{
			printf("%f	", vectorc[i][j]);
		}
		printf("\n\n");
	}

	return 0;
}


void add(float (*ptra)[10][10], float (*ptrb)[10][10], float (*ptrc)[10][10])
{
	int i = 0;
	int j = 0;

	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 10; j++)
		{
			(*ptrc)[i][j] = ((*ptra)[i][j] + (*ptrb)[i][j])/4;

		}
		printf("\n\n");
	}
}

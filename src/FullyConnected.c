#include <stdio.h>
#include <string.h>
#include "FullyConnected.h"


float FullyConnected(float **input, int row, int col);
float FullyConnected(float **input, int row, int col)
{
	float result;
	int r, c;

	for(r = 0; r < row; r++)
	{
		for(c = 0; c < col; c++)
		{
			result = result + input[r][c];
		}
	}
	return result = result/(row*col);
}


#include <stdio.h>
#include <string.h>
#include "Backprop.h"

/*************************** Function Prototypes *********************************/
float MSE(float target, float actual);
void backProp(float **filter, int row, int col, float target, float actual, int LR);
/*********************************************************************************/

float MSE(float target, float actual)
{
	 float result = 0;
	// printf("Target: %f, Actual: %f\n", target, actual);
	 result =.5*(target - actual)*(target - actual);
	 //printf("Error %f\n", result);
	 return result;
}

void backProp(float **filter, int row, int col, float target, float actual, int LR)
{
	float error = MSE(target, actual);
	int r, c;

	for(r = 0; r < row; r++)
	{
		for(c = 0; c < col; c++)
		{
			filter[r][c] = filter[r][c] - LR*error;
		}
	}	
}

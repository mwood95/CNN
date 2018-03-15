#include <stdio.h>
#include <string.h>
#include "Backprop.h"

/*************************** Function Prototypes *********************************/
float MSE(float target, float actual);
void backProp(float **filter, int row, int col, float **image, int ri, int ci, float target, float actual, int LR);
/*********************************************************************************/

float MSE(float target, float actual)
{
	 float result = 0;
	 result =.5*(target - actual)*(target - actual);
	 return result;
}

void backProp(float **filter, int row, int col, float **image, int ri, int ci, float target, float actual, int LR)
{
	float error = MSE(target, actual);
	float result; 
	int r, c;
	int a, b;

	for(r = 0; r < row; r++)
	{
		for(c = 0; c < col; c++)
		{
			result = 0;

			for(a = 0; a < ri; a++)
			{
				for(b = 0; b < ci; b++)
				{
					if(a + r < ci)
					{
						result = result + image[a + r][b + c];
					}
				}
			}
			result = result/(ri*ci);

			if(target == 0)
			{
				filter[r][c] = filter[r][c] - LR*result;
			}
			else
			{
				filter[r][c] = filter[r][c] + LR*result;
			}
		}
	}	
}

/********************************************************************************************
 * Name: 	Michael Wood
 * Date: 	1/23/17
 * File: 	Convolution.c
 *
 * Description:	This file contains the basic functionality that will be implemented on 
 * 		an SoC FPGA for a Convolutional Neural Network (CNN). The purpose of this
 * 		is to provide the basis for what will be implemented on the FPGA. 
 *
 * 		The goal of this program is to take in an arbitary sized image and filter
 * 		and perform a "convolution" to produce an output to be fed into the next
 * 		layer of the CNN.
 *
 ********************************************************************************************/
/* Includes */
#include <stdio.h>
#include <string.h>
#include "FullyConnected.h"
#include "Backprop.h"
#include "RLU.h"


#define DEBUG

/* Function Declarations */
void image_init(float **image, int row, int col);
void filter_init(float **filter, int row, int col);
void frame_shfit(float **image, float **filter, float **out, int len_img, int len_fil, int len_out);
void frame_shift_pool(float **image, float **out, int len_img, int len_frm);
void print_matrix(float **matrix, int row, int col);
float Conv(float **image, float **filter, int row, int col);
float Pool(float **image, int row, int col);


/* Global Variables */

int main(void)
{
	FILE * fd;
	int r, c;
	int len_img;
	int len_fil;
	int len_out;
	int len_out_pool;
	int i = 0;
	float result = .55555;
	float acc;
	int max;

	fd = fopen("Data.txt", "w");


	printf("What size image would you like to start with?\n");
	scanf("%d", &len_img);
	printf("%d x %d\n", len_img, len_img);
	printf("What size filter would you like to use?\n");
	scanf("%d", &len_fil);
	printf("What accuracy are you looking for\n");
	scanf("%f", &acc);
	printf("What is the maximum amount of iterations allowable?\n");
	scanf("%d", &max);
	printf("%d x %d\n", len_fil, len_fil);
	len_out = len_img - len_fil + 1;
	printf("Output %d x %d\n", len_out, len_out);
	len_out_pool = len_out/2;
	printf("Pool Output %d x %d\n" , len_out_pool, len_out_pool);

	// Array of pointers of a given size
	// The later implementation of this will take these lengths as an input
	float *img_ptr[len_img];
	float *fil_ptr[len_fil];
	float *out_ptr[len_out];
	float *out_pool_ptr[len_out_pool];

	// Dynamically allocate 2D arrays for image, filter, and output matrix
	for(i = 0; i < len_img; i++)
	{
		img_ptr[i] = (float *)malloc(len_img*sizeof(float));
	}
	for(i = 0; i < len_fil; i++)
	{
		fil_ptr[i] = (float *)malloc(len_fil*sizeof(float));
	}
	for(i = 0; i < len_out; i++)
	{
		out_ptr[i] = (float *)malloc(len_out*sizeof(float));
	}
	for(i = 0; i < len_out_pool; i++)
	{
		out_pool_ptr[i] = (float *)malloc(len_out_pool*sizeof(float));
	}


	// Initialize Image and Filter Matricies (For Testing Purpose Only)
//	image_init(img_ptr, len_img, len_img);
	filter_init(fil_ptr, len_fil, len_img);
	int iteration = 0;
	float expected;

	while((result) < acc)
	{
		iteration++;
		printf("Populate the image structure\n");
		image_init(img_ptr, len_img, len_img);
//		printf("What is the expected result?\n");
//		scanf("%f", &expected);	
		expected = 1;
		// Run Neural Network
		frame_shift(img_ptr, fil_ptr, out_ptr, len_img, len_fil, len_out);
		RLU(out_ptr, len_out, len_out);
		frame_shift_pool(out_ptr, out_pool_ptr, len_out, 2);
		result = FullyConnected(out_pool_ptr, len_out_pool, len_out_pool);
		printf("Backpropagation Calculation...\n");
		backProp(fil_ptr, len_fil, len_fil, img_ptr, len_img, len_img, expected, result, 1);
		fprintf(fd,"%f \n", result);
		printf("Result: %f\n", result);
		
		if(iteration == 1000)
		{
			printf("\n\nNetwork could not converge on a result.\nProbability after termination: %f\n", result);
//			print_matrix(fil_ptr, len_fil, len_fil);
			break;
		}
		if(result > acc)
		{
			printf("\n\nNetwork converged on a result.\nProbability after termination: %f\n", result);
			printf("It took %d iterations\n", iteration);
//			print_matrix(fil_ptr, len_fil, len_fil);
		}


	}

	fclose(fd);
	return 0;
}

/*******************************************************************************************************************************
 * Function: 	frame_shift_pool
 *
 * Inputs:	image:		A frame of the entire image that needs to be processed.
 * 		out:		Result from convolution (Multiply and Accumulate)	
 * 		len_img:	Size of the Image Matrix (NxN)
 * 		len_frm:	Size of pooling frame to sweep across image (Generally will be 2x2)
 *
 * Description:	This function provides the functionality of "sweeping" a context frame across the input image. 
 * 		The frame should sweep across an arbitrary sized image and pass the data into the pooling  
 * 		computation function. The results will populate an output matrix to give back to the main function.
 * 		
 *
 * *****************************************************************************************************************************/
void frame_shift_pool(float **image, float **out, int len_img, int len_frm)
{
	int r = 0;
	int c = 0;
	int i = 0;
	int row = 0;
	int col = 0;

	float *img_frame_ptr[len_frm];
	for(i = 0; i < len_frm; i++)
	{
		img_frame_ptr[i] = (float *)malloc(len_frm*sizeof(float));
	}

	/******************************************************************
	 * This for loop is unique in that it needs to position the context
	 * frame such that it does not overlap with previous values.
	 * Example:
	 *
	 * [ ] [ ] x x x x . . .   ------>    x x [ ] [ ] x x . . .
	 * [ ] [ ] x x x x . . .   ------>    x x [ ] [ ] x x . . .
	 *  x   x  x x x x . . .   ------>    x x  x   x  x x . . .
	 *  x   x  x x x x . . .   ------>    x x  x   x  x x . . .
	 *
	 * This is the reason for the "len_img + 1" and row and col 
	 * incrementing by two after each iteration. Nested in the final 
	 * two for loops is an if statement that handles the issue with 
	 * accessing the image array elements that are out of bounds.
	 *
	 * For example if our image is 7x7 and our output matrix expects 
	 * a 4x4 solution, the 2x2 pool frame will try to read an 8th 
	 * element within the image matrix that doesn't exist. This 
	 * issue only exists for image inputs that are odd.
	 * ***************************************************************/
	for(row = 0; row < len_img; row = row + 2)
	{
		for(col = 0; col < len_img; col = col + 2)
		{
			for(r = 0; r < len_frm; r++)
			{
				for(c = 0; c < len_frm; c++)
				{
	/*				if( (r + row == 7) | (c + col == 7))
					{	
						(img_frame_ptr)[r][c] == 0;
					}*/
				//	else
				//	{
						(img_frame_ptr)[r][c] = image[r+row][c+col];
				//	}
					
				}
			}
			(out)[(row/2)][(col/2)] = Pool(img_frame_ptr, len_frm, len_frm);
		}
	}	

}


/*******************************************************************************************************************************
 * Function: 	frame_shift	
 *
 * Inputs:	image:		A frame of the entire image that needs to be processed.
 * 		filter:		Filter frame to produce output 
 * 		out:		Result from convolution (Multiply and Accumulate)	
 * 		len_img:	Size of the Image Matrix (NxN)
 * 		len_fil:	Size of the Filter Matrix (NxN)
 * 		len_out:	Size of the Output Matrix (NxN)
 *
 * Description:	This function provides the functionality of "sweeping" a context frame that is the size of the filter across
 * 		the image. The frame should sweep across an arbitrary sized image and pass the data into the convolution
 * 		computation function. The results will populate an output matrix to give back to the main function.
 * 		
 *
 * *****************************************************************************************************************************/


void frame_shift(float **image, float **filter, float **out, int len_img, int len_fil, int len_out)
{
	int r = 0;
	int c = 0;
	int i = 0;
	int row = 0;
	int col = 0;	


	float *img_frame_ptr[len_fil];
	for(i = 0; i < len_fil; i++)
	{
		img_frame_ptr[i] = (float *)malloc(len_fil*sizeof(float));
	}


	for(row = 0; row < len_out; row++)
	{
		for(col = 0; col < len_out; col++)
		{
			for(r = 0; r < len_fil; r++)
			{
				for(c = 0; c < len_fil; c++)
				{
					(img_frame_ptr)[r][c] = image[row + r][col + c];
				}
			}
			(out)[row][col] = Conv(img_frame_ptr, filter, len_fil, len_fil);
		}
	}
}

/*******************************************************************************************************************************
 * Function: 	Conv
 *
 * Inputs:	image:		A frame of the entire image that needs to be processed.
 * 		filter:		Filter frame to produce output 
 * 		row:		The number of rows in the computation
 * 		col:		The number of columns in the computation
 *
 * Description:	This function serves as a model for what will be implemented in hardware later. It is a generic convolution
 * 		calculation that produces a single output. This function will work for any sized image and filter. This means 
 * 		that image frame positioning must be handled outside of this function. This is the basis for how the 
 * 		hardware implementation of this function will work. The CPU will be responsible for positioning the filter
 * 		over image pixel values that are of interest. 
 *
 * *****************************************************************************************************************************/

float Conv(float **image, float **filter, int row, int col)
{

	int r = 0;
	int c = 0;
	float hist;
	float out;
	hist = 0;
	for(r = 0; r < row; r++)
	{
		for(c = 0; c < col; c++)
		{	
			hist = hist + ((filter)[r][c] * (image)[r][c]);
		}

	}

	return out = hist/(row*col);
}


/*******************************************************************************************************************************
 * Function: 	Pool	
 *
 * Inputs:	image:		A frame of the entire image that needs to be processed.
 * 		row:		The number of rows in the computation
 * 		col:		The number of columns in the computation
 *
 * Description:	This function serves as a model for what will be implemented in hardware later. It is a generic pooling
 * 		function that passes the greatest value in the context frame to the output. This function will work for 
 * 		any sized image and filter. This means that image frame positioning must be handled outside of this function. 
 * 		This is the basis for how the hardware implementation of this function will work. The CPU will be responsible
 * 		for positioning the filter over image pixel values that are of interest. 			
 *
 * *****************************************************************************************************************************/
float Pool(float **image, int row, int col)
{
	int r = 0;
	int c = 0;
	float out = 0;

	for(r = 0; r < row; r++)
	{
		for(c = 0; c < col; c++)
		{	
			if(out < image[r][c])
			{
				out = image[r][c];
			}
			else
			{
				out = out;
			}
		}
	}

	return out;

}

void image_init(float **image, int row, int col)
{
	int r = 0;
	int c = 0;
	float buff;

	for(r = 0; r < row; r++)
	{
		for(c = 0; c < col; c++)
		{

			//scanf("%f", &buff);
			if(r == c)
			{
				(image)[r][c] = 1; //buff;
			}
			else
			{
				(image)[r][c] = 0; //buff;
			}
//			printf("%f	", image[r][c]);
			
		}
	//	printf("\n\n");
	}
} 


void print_matrix(float **matrix, int row, int col)
{
	int r = 0;
	int c = 0;
	for(r = 0; r < row; r++)
	{
		for(c = 0; c < col; c++)
		{
			printf("%f	", matrix[r][c]);
		}
		printf("\n\n");
	}
}

void filter_init(float **filter, int row, int col)
{
	int r = 0;
	int c = 0;
	for(r = 0; r < row; r++)
	{
		for(c = 0; c < col; c++)
		{
			if(r == c)
			{
				(filter)[r][c] = 1;
			}
			else
			{
				(filter)[r][c] = -1;
			}
		}
	}
}



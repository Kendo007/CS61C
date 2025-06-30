/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should update given color with new Color.
void evaluateOnePixel(Color* color, Image *image, int row, int col) {
	if (image->image[row][col].B & 1)
		color->R = color->G = color->B = 255;
	else
		color->R = color->G = color->B = 0;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Image *steganography = malloc(sizeof(Image));

	if (steganography == NULL) {
		freeImage(image);
		exit(-1);
	}

	const uint8_t rows = image->rows, cols = image->cols;

	steganography->rows = rows;
	steganography->cols = cols;
	steganography->image = malloc(rows * sizeof(Color*));

	if (steganography->image == NULL) {
		free(steganography);
		freeImage(image);
		exit(-1);
	}

	for (int i = 0; i < image->rows; i++) {
		steganography->image[i] = malloc(cols * sizeof(Color));

		if (steganography->image[i] == NULL) {
			for (int j = 0; j < i; j++)
				free(steganography->image[j]);

			free(steganography->image);
			free(steganography);
			freeImage(image);

			exit(-1);
		}
	}

	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			evaluateOnePixel(steganography->image[i] + j, image, i, j);

	return steganography;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv) {
	if (argc < 2)
		exit(-1);

	Image *image = readData(argv[1]);
	Image *decoded = steganography(image);

	freeImage(image);
	writeData(decoded);
	freeImage(decoded);

	return 0;
}

/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE *file = fopen(filename, "r");

	int rows, cols, scale;
	Image *image = malloc(sizeof(Image));

	if (file != NULL && image != NULL) {
		char skip[2];

		fscanf(file, "%s", skip);
		fscanf(file, "%d %d", &cols, &rows);
		fscanf(file , "%d", &scale);

		image->cols = cols;
		image->rows = rows;
		image->image = malloc(rows*sizeof(Color*));

		if (image->image == NULL) {
			free(image);
			exit(-1);
		}

		for (int i = 0; i < rows; i++) {
			image->image[i] = malloc(cols*sizeof(Color));

			if (image->image[i] == NULL) {
				for (int j = 0; j < i; j++)
					free(image->image[j]);

				free(image->image);
				free(image);
				exit(-1);
			}
		}

		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				fscanf(file, "%hhu %hhu %hhu",
					   &image->image[i][j].R,
					   &image->image[i][j].G,
					   &image->image[i][j].B);
			}
		}
	} else {
		free(image);
		exit(-1);
	}

	fclose(file);

	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("P3\n");
	printf("%d %d\n255\n", image->cols, image->rows);

	const uint8_t rows = image->rows, cols = image->cols;
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			printf("%3hhu %3hhu %3hhu",
				image->image[i][j].R,
				image->image[i][j].G,
				image->image[i][j].B);

			if (j != cols - 1)
				printf("   ");
			else
				printf("\n");
		}
	}
}

//Frees an image
void freeImage(Image *image)
{
	for (int i = 0; i < image->rows; ++i)
		free(image->image[i]);

	free(image->image);
	free(image);
}
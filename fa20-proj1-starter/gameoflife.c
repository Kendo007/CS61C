/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
void evaluateOneCell(Color* color, Image *image, int row, int col, uint32_t rule) {
	uint8_t countR, countG, countB;

	color->R = color->G = color->B = 0;

	for (int i = 0; i < 8; ++i) {
		countR = countG = countB = 0;

		uint8_t mask = 1 << i;

		uint32_t rows = image->rows;
		uint32_t cols = image->cols;

		for (int8_t dx = -1; dx <= 1; ++dx) {
			for (int8_t dy = -1; dy <= 1; ++dy) {
				// Skipping current row and col
				if (dx == 0 && dy == 0)
					continue;

				int32_t nx = (row + dx + (int32_t)rows) % (int32_t)rows;
				int32_t ny = (col + dy + (int32_t)cols) % (int32_t)cols;

				countR += (image->image[nx][ny].R & mask) ? 1 : 0;
				countG += (image->image[nx][ny].G & mask) ? 1 : 0;
				countB += (image->image[nx][ny].B & mask) ? 1 : 0;
			}
		}

		// Evaluate red
		uint8_t idx = (image->image[row][col].R & mask) ? 9 + countR : countR;
		if (rule & (1 << idx))
			color->R |= mask;

		// Evaluate green
		idx = (image->image[row][col].G & mask) ? 9 + countG : countG;
		if (rule & (1 << idx))
			color->G |= mask;

		// Evaluate blue
		idx = (image->image[row][col].B & mask) ? 9 + countB : countB;
		if (rule & (1 << idx))
			color->B |= mask;
	}
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *gameOfLife = malloc(sizeof(Image));

	if (gameOfLife == NULL) {
		freeImage(image);
		exit(-1);
	}

	const uint8_t rows = image->rows, cols = image->cols;

	gameOfLife->rows = rows;
	gameOfLife->cols = cols;
	gameOfLife->image = malloc(rows * sizeof(Color*));

	if (gameOfLife->image == NULL) {
		free(gameOfLife);
		freeImage(image);
		exit(-1);
	}

	for (int i = 0; i < image->rows; i++) {
		gameOfLife->image[i] = malloc(cols * sizeof(Color));

		if (gameOfLife->image[i] == NULL) {
			for (int j = 0; j < i; j++)
				free(gameOfLife->image[j]);

			free(gameOfLife->image);
			free(gameOfLife);
			freeImage(image);

			exit(-1);
		}
	}

	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			evaluateOneCell(gameOfLife->image[i] + j, image, i, j, rule);

	return gameOfLife;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("usage: ./%s filename rule\n", argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");

		exit(-1);
	}

	Image *image = readData(argv[1]);
	Image *decoded = life(image, strtol(argv[2], NULL, 16));

	freeImage(image);
	writeData(decoded);
	freeImage(decoded);
}

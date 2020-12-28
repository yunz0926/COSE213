#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 15

int count, size;

void magicsquare(int square[][MAX_SIZE], int i, int j);

void main(void)
{
	int square[MAX_SIZE][MAX_SIZE];
	int i, j;
	int row = 0, column = 0;

	printf("Enter the size of the square: ");
	scanf("%d", &size);

	if (size < 1 || size > MAX_SIZE + 1) {
		fprintf(stderr, "Error! Size is out of range\n");
		exit(EXIT_FAILURE);
	}
	if (!(size % 2)) {
		fprintf(stderr, "Error! Size is even\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			square[i][j] = 0;
	square[0][(size - 1) / 2] = 1;
	i = 0;
	j = (size - 1) / 2;
	count = 2;
	magicsquare(square, i, j);

	printf(" Magic Square of size %d : \n\n", size);
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++)
			printf("%5d", square[i][j]);
		printf("\n");
	}
	printf("\n\n");
}

void magicsquare(int square[][MAX_SIZE], int i, int j)
{
	int row, column;
	row = (i - 1 < 0) ? (size - 1) : (i - 1);
	column = (j - 1 < 0) ? (size - 1) : (j - 1);
	if (square[row][column])
		i = (++i) % size;
	else {
		i = row;
		j = (j - 1 < 0) ? (size - 1) : --j;
	}
	square[i][j] = count;
	if (count == size * size)
		return;
	count++;

	magicsquare(square, i, j);
}
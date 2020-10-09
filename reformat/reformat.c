#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIM_ROWS 1024
#define LIM_COLS 1024

void reformat(FILE *fin, FILE *fout, int cols)
{
	char buf[LIM_COLS];
	char *token;
	int j = 1;
	while(fgets(buf, sizeof buf, fin) != NULL)
	{
		token = strtok(buf, " ");
		while(token != NULL)
		{
			fprintf(fout, "%s%c", token, (j == cols && (j = 1)) ? '\n':' ');
			token = strtok(NULL, " ");
			if(token != NULL) /* remove \n */
				token[strcspn(token, "\n")] = 0;
			++j;
		}
	}
}

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("Usage: %s input.pgm output.pgm\n", argv[0]);
		return 1;
	}

	if(strcmp(argv[1], argv[2]) == 0)
	{
		printf("Input and output file may not be the same\n");
		return 1;
	}

	FILE *fin = fopen(argv[1], "r");
	if(fin == NULL)
	{
		printf("Unable to read %s\n", argv[1]);
		return 1;
	}
	FILE *fout = fopen(argv[2], "w");
	if(fout == NULL)
	{
		printf("Unable to write to %s\n", argv[2]);
		return 1;
	}

	char magic[4]; /* magic number, should be P2 */
	fgets(magic, sizeof magic, fin);
	magic[2] = '\0'; /* chop \n */
	if(strcmp(magic, "P2") != 0)
	{
		if(strcmp(magic, "P5") == 0)
			printf("%s is a compressed PGM file. This utility \
			can only read uncompressed PGM files\n", argv[1]);
		else
			printf("%s: invalid PGM file, %s is not the right \
			magic number\n", argv[1], magic);
		return 1;
	}


	int rows, cols;
	char buf[16]; /* longest valid string has length 11 */
	fgets(buf, sizeof buf, fin);

	char *token = strtok(buf, " ");
	if(token != NULL) rows = atoi(token);
	else return 1;

	token = strtok(NULL, " ");
	if(token != NULL) cols = atoi(token);
	else return 1;

	if(rows > LIM_ROWS || cols > LIM_COLS)
	{
		printf("Image is too large, max dimensions are \
			%d x %d\n", rows, cols);
		return 1;
	}

	fgets(buf, sizeof buf, fin);
	int max_gray = atoi(buf);

	fprintf(fout, "%s\n%d %d\n%d\n", magic, rows, cols, max_gray);
	reformat(fin, fout, cols);

	return 0;
}

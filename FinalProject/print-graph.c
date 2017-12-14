/*
 * This program prints a matrix binary file that first lists the dimension
 * followed by the values
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "graph.h"

#define NOT_CONNECTED 999999999

int main(int argc, char **argv)
{
	int num;
	float **vals;
	char *print_file = NULL;

	if (argc > 2) 
	{
		printf("\nIncorrect usage. Exiting\n");
		exit(1);
	}

	if (argc == 2)
	{
		print_file = (char *) malloc (sizeof(char) * 100);
		strcpy(print_file, argv[1]);
	}

	if (print_file == NULL)
	{
		print_file = (char *) malloc (sizeof(char) * 100);
		strcpy(print_file, "default-make-graph-file.dat");
	}

	read_graph(print_file, &num, &vals);

	print_graph(num, vals);

	// for (i = 0; i < num; i++)
	// {
	// 	free(vals[i]);
	// }
	free(vals);
	free(print_file);

	return 0;
}

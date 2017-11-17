/*
 * This program performs floyds algorithm on a matrix binary file 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "graph.h"

#define NOT_CONNECTED 999999999

int main(int argc, char **argv)
{
	int num, k , i, j;
	int **dist;
	char *in_file = NULL, *out_file = NULL;
	clock_t tp1, tp2, beg, end;

	if (argc > 3) 
	{
		printf("\nIncorrect usage. Exiting\n");
		exit(1);
	}

	if (argc == 3)
	{
		in_file = (char *) malloc (sizeof(char) * 100);
		strcpy(in_file, argv[1]);
		out_file = (char *) malloc (sizeof(char) * 100);
		strcpy(out_file, argv[2]);
	}
	else
	{
		printf("\nUsage: ./floyd-serial input_file output_file\n");
		exit(1);
	}

	beg = clock();

	read_graph(in_file, &num, &dist);

	tp1 = clock();

	for (k = 0; k < num; k++)
	{
		for (i = 0; i < num; i++)
		{
			for (j = 0; j < num; j++)
			{
				if (dist[i][k] + dist[k][j] < dist[i][j])
				{
					dist[i][j] = dist[i][k] + dist[k][j];
				}
			}
		}
	}

	tp2 = clock();

	write_graph(out_file, num, dist);

	end = clock();

	printf("floyd-serial execution time:\n");
	printf("\tn = %d nodes\n", num);
	printf("\tp = 1 cpus\n");
	printf("\tptime = %8f secs\n", ((double)(end - beg)) / CLOCKS_PER_SEC);
	printf("\tftime = %8f secs\n\n", ((double)(tp2 - tp1)) / CLOCKS_PER_SEC);

	return 0;
}

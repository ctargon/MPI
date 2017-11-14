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
	struct timespec	tp1, tp2, beg, end;

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

	clock_gettime(CLOCK_REALTIME, &beg);

	read_graph(in_file, &num, &dist);

	clock_gettime(CLOCK_REALTIME, &tp1);

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

	clock_gettime(CLOCK_REALTIME,&tp2);

	write_graph(out_file, num, dist);

	clock_gettime(CLOCK_REALTIME, &end);

	printf("floyd-serial execution time:\n");
	printf("\tn = %d nodes\n", num);
	printf("\tp = 1 cpus\n");
	printf("\tptime = %8f secs\n", (end.tv_nsec - beg.tv_nsec) / 1000000000.0);
	printf("\tftime = %8f secs\n\n", (tp2.tv_nsec-tp1.tv_nsec) / 1000000000.0);

	return 0;
}

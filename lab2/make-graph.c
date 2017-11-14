/*
 * This program generates a vector binary file that first lists the number of 
 * elements in the vector followed by the values
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
	int *r_nums, opt, r_int = 0, num = -1, i, j, val = -1, prob = -1;
	char *out_file = NULL;

	// seed the random number generator
	srand(time(NULL));

	while ((opt = getopt(argc, argv, "n:r:p:o:")) != -1)
	{
		switch (opt)
		{
			case 'n':
				num = atoi(optarg);
				if (num <= 0)
				{
					printf("Invalid number of elements, exiting\n");
					exit(1);
				}
				break;
			case 'r':
				val = atoi(optarg);
				if (val <= 0)
				{
					printf("Invalid entry for possible values, exiting\n");
					exit(1);
				}
				break;
			case 'p':
				prob = atoi(optarg);
				if (prob <= 0)
				{
					printf("Invalid entry for possible values, exiting\n");
					exit(1);
				}
				break;
			case 'o':
				out_file = (char *) malloc (sizeof(char) * 100);
				strcpy(out_file, optarg);
				break;
			case '?':
				printf("Incorrect argument(s), exiting\n");
				exit(1);
		}
	}

	if (num == -1) num = 5; // default number if argument is not provided
	if (out_file == NULL)
	{
		out_file = (char *) malloc (sizeof(char) * 100);
		strcpy(out_file, "default-make-graph-file.dat");
	} 

	r_nums = (int *) malloc (sizeof(int) * num * num);
	
	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
		{
			if (i == j) r_nums[i * num + j] = 0;
			else
			{
				r_int = rand() % prob + 1;
				if (r_int < val) r_nums[i * num + j] = r_int;
				else r_nums[i * num + j] = NOT_CONNECTED;
			}
		}
	}

	write_graph(out_file, num, &r_nums);

	free(r_nums);
	free(out_file);

	return 0;
}

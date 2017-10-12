/*
 * This program generates a vector binary file that first lists the number of 
 * elements in the vector followed by the values
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv)
{
	int opt, num = -1, i;
	double *r_nums;
	char *out_file = NULL;
	FILE *fp = NULL;

	// seed the random number generator
	srand(time(NULL));

	while ((opt = getopt(argc, argv, "n:o:")) != -1)
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
			case 'o':
				out_file = (char *) malloc (sizeof(char) * 100);
				strcpy(out_file, optarg);
				break;
			case '?':
				printf("Incorrect argument(s), exiting\n");
				exit(1);
		}
	}

	if (num == -1) num = 100; // default number if argument is not provided
	if (out_file == NULL)
	{
		out_file = (char *) malloc (sizeof(char) * 100);
		strcpy(out_file, "default-list-file.dat");
	} 

	r_nums = (double *) malloc (sizeof(double) * num);
	
	for (i = 0; i < num; i++)
	{
		r_nums[i] = (double)rand() / RAND_MAX;
	}

	fp = fopen(out_file, "wb");
	fwrite(&num, sizeof(int), 1, fp);
	fwrite(r_nums, sizeof(double), num, fp);
	fclose(fp);

	free(r_nums);
	free(out_file);

	return 0;
}

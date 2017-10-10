/*
 * This program adds a vector binary file that first lists the number of 
 * elements in the vector followed by the values
 * Parallel implemntation using MPI
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>

int IsPowerOfTwo(int x);

int main(int argc, char **argv)
{
	int i, opt, num, *vals, sum = 0, size, rank;
	char *print_file = NULL;
	FILE *fp = NULL;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (IsPowerOfTwo(size) == 0)
	{
		printf("Processor count must be 2, 4, 8, 16, or 32\n");
		MPI_Finalize();
		exit(1);
	}


	while ((opt = getopt(argc, argv, "i:")) != -1)
	{
		switch (opt)
		{
			case 'i':
				print_file = (char *) malloc (sizeof(char) * 100);
				strcpy(print_file, optarg);
				break;
			case '?':
				printf("Incorrect argument(s), exiting\n");
				exit(1);
		}
	}

	if (print_file == NULL)
	{
		print_file = (char *) malloc (sizeof(char) * 100);
		strcpy(print_file, "default-list-file.dat");
	}

	if ((fp = fopen(print_file, "rb")) == NULL)
	{
		printf("Cannot open file %s for adding, exiting\n", print_file);
		exit(1);
	}

	fread(&num, sizeof(int), 1, fp);
	vals = (int *) malloc (sizeof(int) * num);
	fread(vals, sizeof(int), num, fp);

	for (i = 0; i < num; i++)
	{
		sum += vals[i];
	}

	printf("%d\n", sum);

	free(vals);
	free(print_file);

	return 0;
}


int IsPowerOfTwo(int x)
{
    return (x & (x - 1)) == 0;
}










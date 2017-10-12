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
#include "MyMPI.h"

int IsPowerOfTwo(int x);

int main(int argc, char **argv)
{
	int i, opt, len = 0, size, rank;
	double *vals, sum = 0, glob_sum = 0;
	char *file = NULL;
	void *inputs;
	MPI_Status status;
	//FILE *fp = NULL;

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
				file = (char *) malloc (sizeof(char) * 100);
				strcpy(file, optarg);
				break;
			case '?':
				printf("Incorrect argument(s), exiting\n");
				MPI_Finalize();
				exit(1);
		}
	}

	if (file == NULL)
	{
		file = (char *) malloc (sizeof(char) * 100);
		strcpy(file, "default-list-file.dat");
	}

	read_block_vector(file, &inputs, MPI_DOUBLE, &len, MPI_COMM_WORLD);

	if (rank == 0)
	{
		vals = (double *) inputs;
		for (i = 0; i < BLOCK_SIZE(rank, size, len); i++)
		{
			glob_sum += vals[i];
		}

		for (i = 1; i < size; i++)
		{
			MPI_Recv(&sum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			glob_sum += sum;
		}
		printf("%f\n", glob_sum);
	}
	else
	{
		vals = (double *) inputs;

		for (i = 0; i < BLOCK_SIZE(rank, size, len); i++)
		{
			sum += vals[i];
		}	
		MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}

	//printf("process %d has sum of %f\n", rank, sum);

	return 0;
}


int IsPowerOfTwo(int x)
{
    return (x & (x - 1)) == 0;
}










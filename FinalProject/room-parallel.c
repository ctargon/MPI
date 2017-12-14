/*
 * This program adds a vector binary file that first lists the number of 
 * elements in the vector followed by the values
 * Parallel implemntation using MPI
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>
#include "MyMPI.h"

void floyd (int ***subs, int *coords, int *dims, MPI_Comm row, MPI_Comm col, int nnodes);
int IsPowerOfTwo(int x);

int main(int argc, char **argv)
{
	int rows, cols, size, rank, dim_size[2], periodic[2], grid_coords[2];
	char *in_file = NULL, *out_file = NULL;
	void **subs, *storage;
	MPI_Comm cart_comm, r_comm, c_comm;
	FILE *fp;
	//MPI_Status status;
	clock_t tp1, tp2, beg, end;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// if (IsPowerOfTwo(size) == 0)
	// {
	// 	printf("Processor count must be 2, 4, 8, 16, or 32\n");
	// 	MPI_Finalize();
	// 	exit(1);
	// }

	if (argc == 3)
	{
		in_file = (char *) malloc (sizeof(char) * 100);
		strcpy(in_file, argv[1]);
		out_file = (char *) malloc (sizeof(char) * 100);
		strcpy(out_file, argv[2]);
	}
	else
	{
		printf("\nUsage: ./floyd-parallel input_file output_file\n");
		exit(1);
	}

	beg = clock();

	dim_size[0] = dim_size[1] = 0;
	MPI_Dims_create(size, 2, dim_size);

	periodic[0] = periodic[1] = 0;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dim_size, periodic, 1, &cart_comm);

	MPI_Cart_coords(cart_comm, rank, 2, grid_coords);

	//printf("%d (%d %d) (%d %d)\n", rank, grid_coords[0], grid_coords[1], dim_size[0], dim_size[1]);

	MPI_Comm_split(cart_comm, grid_coords[0], grid_coords[1], &r_comm);
	MPI_Comm_split(cart_comm, grid_coords[1], grid_coords[0], &c_comm);

	read_checkerboard_matrix(in_file, &subs, &storage, MPI_INT, &rows, &cols, cart_comm);
	// debug print
	//print_checkerboard_matrix(subs, MPI_INT, rows, cols, cart_comm);

	int **mat = (int **)subs;

	tp1 = clock();
	floyd(&mat, grid_coords, dim_size, r_comm, c_comm, rows);
	tp2 = clock();

	// write header of file
	fp = fopen(out_file, "wb");
	fwrite(&rows, sizeof(int), 1, fp);
	fwrite(&cols, sizeof(int), 1, fp);

	subs = (void **)mat;

	write_checkerboard_matrix(subs, MPI_INT, rows, cols, cart_comm, fp);
	end = clock();

	if (rank == 0)
	{
		printf("floyd-serial execution time:\n");
		printf("\tn = %d nodes\n", rows);
		printf("\tp = %d cpus\n", size);
		printf("\tptime = %8f secs\n", ((double)(end - beg)) / CLOCKS_PER_SEC);
		printf("\tftime = %8f secs\n\n", ((double)(tp2 - tp1)) / CLOCKS_PER_SEC);

	}




	MPI_Finalize();
	//printf("process %d has sum of %f\n", rank, sum);

	return 0;
}

void floyd (int ***subs, int *coords, int *dims, MPI_Comm rowcom, MPI_Comm colcom, int nnodes)
{
	int k, i, j, local_r, local_c, r, c;

	int *buf_r = (int *) malloc (BLOCK_SIZE(coords[1], dims[1], nnodes) * sizeof(int));
	int *buf_c = (int *) malloc (BLOCK_SIZE(coords[0], dims[0], nnodes) * sizeof(int));

	for (k = 0; k < nnodes; k++)
	{
		if (BLOCK_OWNER(k, dims[0], nnodes) == coords[0])
		{
			local_r = k - BLOCK_LOW(coords[0], dims[0], nnodes);
			//memcpy(buf_r, (*subs)[local_r], BLOCK_SIZE(coords[1], dims[1], nnodes) * sizeof(int));
			for (r = 0; r < BLOCK_SIZE(coords[1], dims[1], nnodes); r++) buf_r[r] = (*subs)[local_r][r];
		}

		//printf("owner is %d coord is %d\n", BLOCK_OWNER(k, dims[0], nnodes), coords[0]);

		if (BLOCK_OWNER(k, dims[1], nnodes) == coords[1])
		{
			local_c = k - BLOCK_LOW(coords[1], dims[1], nnodes);

			//printf("block size is %d\n", BLOCK_SIZE(coords[0], dims[0], nnodes));

			for (c = 0; c < BLOCK_SIZE(coords[0], dims[0], nnodes); c++) buf_c[c] = (*subs)[c][local_c];
		}

		//printf("Hello! from %d %d aka block owner %d\n", coords[0], coords[1], BLOCK_OWNER(k, dims[0], nnodes));

		MPI_Bcast(buf_r, BLOCK_SIZE(coords[1], dims[1], nnodes), MPI_INT, BLOCK_OWNER(k, dims[0], nnodes), colcom);

		//printf("------------size %d----------------\n", BLOCK_SIZE(coords[0], dims[0], nnodes));

		MPI_Bcast(buf_c, BLOCK_SIZE(coords[0], dims[0], nnodes), MPI_INT, BLOCK_OWNER(k, dims[1], nnodes), rowcom);

		//printf("------------swag %d----------------\n", BLOCK_SIZE(coords[0], dims[0], nnodes));

		for (i = 0; i < BLOCK_SIZE(coords[0], dims[0], nnodes); i++)
		{
			for (j = 0; j < BLOCK_SIZE(coords[1], dims[1], nnodes); j++)
			{
				if (buf_r[j] + buf_c[i] < (*subs)[i][j]) (*subs)[i][j] = buf_r[j] + buf_c[i];
			}
		}
	}
}

int IsPowerOfTwo(int x)
{
    return (x & (x - 1)) == 0;
}










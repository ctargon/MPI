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
#include <math.h>
#include "graph.h"
#include "MyMPI.h"

#define J 31
#define K 63 

int IsPowerOfTwo(int x);
float roommate_assign(int num, float **dist, int **fib_list);
int * random_assignment(int num, float **dist, int **fib_list);
float get_sum (int *a, float **dist, int num);

int main(int argc, char **argv)
{
	float **dist, cost, min, local_min;
	int trials, num, rank, size, i, *init_nums = (int *) malloc (sizeof(int) * K);
	char *in_file = NULL;
	//MPI_Status status;
	clock_t tp1, tp2;//, beg, end;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (argc == 3)
	{
		in_file = (char *) malloc (sizeof(char) * 100);
		strcpy(in_file, argv[1]);
		trials = atoi(argv[2]);
	}
	else
	{
		printf("\nUsage: ./floyd-parallel input_file\n");
		exit(1);
	}

	//beg = clock();

	// read in the graph to each process
	read_graph(in_file, &num, &dist);

	if (rank == 0) tp1 = clock();

	srand(time(NULL) / (rank + 1));

	for (i = 0; i < K; i++)
		init_nums[i] = rand();

	//MPI_Bcast(&init_nums, K, MPI_INT, 0, MPI_COMM_WORLD);
	local_min = 999999.0;
	for (i = 0; i < trials; i++)
	{
		cost = roommate_assign(num, dist, &init_nums);
		if (cost < local_min) local_min = cost;
	}
	

	MPI_Barrier(MPI_COMM_WORLD);

	printf("rank %d has local minima %f\n", rank, local_min);

	MPI_Reduce(&local_min, &min, 1, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);

	if (rank == 0) tp2 = clock();

	if (rank == 0)
	{
		printf("minimum cost is: %f\n", min);
		printf("\tcompute time = %8f secs\n", ((double)(tp2 - tp1)) / CLOCKS_PER_SEC);
	}


	



	//end = clock();

	// if (rank == 0)
	// {
	// 	printf("floyd-serial execution time:\n");
	// 	printf("\tn = %d nodes\n", rows);
	// 	printf("\tp = %d cpus\n", size);
	// 	printf("\tptime = %8f secs\n", ((double)(end - beg)) / CLOCKS_PER_SEC);
	// 	printf("\tftime = %8f secs\n\n", ((double)(tp2 - tp1)) / CLOCKS_PER_SEC);

	// }

	MPI_Finalize();
	//printf("process %d has sum of %f\n", rank, sum);

	return 0;
}

int LFG(int **fib_list)
{
	int i, out; 

	for (i = 0; i < K; i++)
	{
		if (i < K - 1) 
		{
			if (i == 0) out = (*fib_list)[J - 1] ^ (*fib_list)[K - 1];
			(*fib_list)[i] = (*fib_list)[i + 1];
		}
		else (*fib_list)[i] = out;
	}

	return out;
}


float roommate_assign(int num, float **dist, int **fib_list)
{
	int *a = random_assignment(num, dist, fib_list);

	float t = 1, u1, u2, sum = get_sum(a, dist, num);

	int c1, c2;
	int c = 0;

	while (c < 1000)
	{
		do 
		{
			u1 = ((float)LFG(fib_list) / (float)RAND_MAX);
			u2 = ((float)LFG(fib_list) / (float)RAND_MAX);
			if (u1 >= 1 || u2 >= 1) printf("uh oh\n");
			c1 = u1 * num;
			c2 = u2 * num;
		} while (a[c1] == a[c2]);

		int swap, *temp_a = (int *) malloc (sizeof(int) * num);
		memcpy(temp_a, a, sizeof(int) * num);

		swap = temp_a[c1];
		temp_a[c1] = temp_a[c2];
		temp_a[c2] = swap;

		float new_sum = get_sum(temp_a, dist, num);

		// printf("u1 is %0.4f and exp is %0.4f\n", u1, exp((sum - new_sum) / (float)t));

		if (new_sum < sum || u1 <= exp((sum - new_sum) / t))
		{
			swap = a[c1];
			a[c1] = a[c2];
			a[c2] = swap;
			c = 0;
			sum = new_sum;
		}
		else c++;


		t *= 0.999;
	}

	return sum;
}
	

int * random_assignment(int num, float **dist, int **fib_list)
{
	int *a = (int *) calloc (sizeof(int), num);
	int room, i, j, occur, flag;

	for (i = 0; i < num; i++)
	{
		do 
		{
			occur = flag = 0;
			room = LFG(fib_list) % (num / 2);

			for (j = 0; j < i; j++)
			{
				if (a[j] == room) occur++;
				if (occur >= 2) 
				{
					flag = 1;
					break;
				}
			}
		} while (flag == 1);

		a[i] = room;
	}
	return a;
}

float get_sum (int *a, float **dist, int num)
{
	int i, j;
	float sum = 0;

	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
		{
			if (a[i] == a[j]) sum += dist[i][j];
		}
	}

	return sum;
}


int IsPowerOfTwo(int x)
{
    return (x & (x - 1)) == 0;
}










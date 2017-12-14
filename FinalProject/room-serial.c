/*
 * This program performs floyds algorithm on a matrix binary file 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "graph.h"

int * random_assignment(int num, float **dist);
float get_sum (int *a, float **dist, int num);

int main(int argc, char **argv)
{
	int num, i, *a;
	float **dist;
	char *in_file = NULL, *out_file = NULL;
	clock_t tp1, tp2, beg, end;

	srand(time(NULL));

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

	a = random_assignment(num, dist);

	float t = 1, u1, u2, sum = get_sum(a, dist, num);

	int c1, c2;
	int c = 0;

	while (c < 1000)
	{
		do 
		{
			u1 = ((float)rand() / (float)RAND_MAX);
			u2 = ((float)rand() / (float)RAND_MAX);
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


	tp2 = clock();

	for (i = 0; i < num; i++)
	{
		printf("Student %d assigned room %d\n", i, a[i]);
	}

	printf("total cost: %f\n", sum);

	write_graph(out_file, num, dist);

	end = clock();

	printf("room-serial execution time:\n");
	printf("\tn = %d nodes\n", num);
	printf("\tp = 1 cpus\n");
	printf("\tptime = %8f secs\n", ((double)(end - beg)) / CLOCKS_PER_SEC);
	printf("\tftime = %8f secs\n\n", ((double)(tp2 - tp1)) / CLOCKS_PER_SEC);

	return 0;
}


int * random_assignment(int num, float **dist)
{
	int *a = (int *) calloc (sizeof(int), num);
	int room, i, j, occur, flag;

	for (i = 0; i < num; i++)
	{
		do 
		{
			occur = flag = 0;
			room = rand() % (num / 2);

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




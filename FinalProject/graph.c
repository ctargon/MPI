// functions for graph operations

#include "graph.h"

void read_graph(char *file_name, int *n, float ***A)
{
	FILE *fp = NULL;
	int dim, i;
	float *storage = NULL, **array;

	if ((fp = fopen(file_name, "rb")) == NULL)
	{
		printf("Cannot open file %s, exiting\n", file_name);
		exit(1);
	}

	fread(&dim, sizeof(int), 1, fp);
	fread(&dim, sizeof(int), 1, fp);
	*n = dim;

	printf("Reading graph from file %s\n", file_name);

	storage = (float *) malloc (sizeof(float) * dim * dim);
	fread(storage, sizeof(float), dim * dim, fp);

	array = (float **) malloc (sizeof(float *) * dim);

	for (i = 0; i < (*n); i++)
	{
		array[i] = &storage[i * dim];
	}

	*A = array;
}

void write_graph(char *file_name, int n, float **A)
{
	FILE *fp;

	printf("Writing graph to file %s\n", file_name);

	fp = fopen(file_name, "wb");
	fwrite(&n, sizeof(int), 1, fp);
	fwrite(&n, sizeof(int), 1, fp);
	fwrite(*A, sizeof(float), n * n, fp);
	fclose(fp);
}

void print_graph(int n, float **A)
{
	int i, j;

	printf("Graph is a %d x %d matrix\n\n", n, n);

	printf("    | ");
	for (i = 0; i < n; i++)
	{
		printf(" %4d ", i);
	}
	printf("\n    | ");

	for (i = 0; i < n; i++)
	{
		printf("------");
	}
	printf("\n");

	for (i = 0; i < n; i++)
	{
		printf(" %2d | ", i);
		for (j = 0; j < n; j++)
		{
			printf(" %0.4f ", A[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

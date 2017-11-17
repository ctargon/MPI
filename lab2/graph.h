// function prototypes for graph operations

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NOT_CONNECTED 999999999

void read_graph(char *file_name, int *n, int ***A);

void write_graph(char *file_name, int n, int **A);

void print_graph(int n, int **A);

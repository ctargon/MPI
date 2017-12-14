// function prototypes for graph operations

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void read_graph(char *file_name, int *n, float ***A);

void write_graph(char *file_name, int n, float **A);

void print_graph(int n, float **A);

// function prototypes for graph operations

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NOT_CONNECTED 999999999

void read_graph(char *file_name, int *n, int ***A);

void write_graph(char *file_name, int n, int **A);

void print_graph(int n, int **A);

void read_checkerboard_matrix (
   char *s,              /* IN - File name */
   void ***subs,         /* OUT - 2D array */
   void **storage,       /* OUT - Array elements */
   MPI_Datatype dtype,   /* IN - Element type */
   int *m,               /* OUT - Array rows */
   int *n,               /* OUT - Array cols */
   MPI_Comm grid_comm);   /* IN - Communicator */

void print_checkerboard_matrix (
   void       **a,            /* IN -2D matrix */
   MPI_Datatype dtype,        /* IN -Matrix element type */
   int          m,            /* IN -Matrix rows */
   int          n,            /* IN -Matrix columns */
   MPI_Comm     grid_comm);



// functions for graph operations

#include "graph.h"

void read_graph(char *file_name, int *n, int ***A)
{
	FILE *fp = NULL;
	int dim, i, *storage = NULL;
	int **array;

	if ((fp = fopen(file_name, "rb")) == NULL)
	{
		printf("Cannot open file %s, exiting\n", file_name);
		exit(1);
	}

	fread(&dim, sizeof(int), 1, fp);
	*n = dim;

	printf("Reading graph from file %s\n", file_name);

	storage = (int *) malloc (sizeof(int) * dim * dim);
	fread(storage, sizeof(int), dim * dim, fp);

	array = (int **) malloc (sizeof(int *) * dim);

	for (i = 0; i < (*n); i++)
	{
		array[i] = &storage[i * dim];
	}

	*A = array;
}

void write_graph(char *file_name, int n, int **A)
{
	FILE *fp;

	printf("Writing graph to file %s\n", file_name);

	fp = fopen(file_name, "wb");
	fwrite(&n, sizeof(int), 1, fp);
	fwrite(*A, sizeof(int), n * n, fp);
	fclose(fp);
}

void print_graph(int n, int **A)
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
			if (A[i][j] == NOT_CONNECTED)
			{
				printf(" %4d ", -1);
			}
			else
			{
				printf(" %4d ", A[i][j]);
			}
		}
		printf("\n");
	}
	printf("\n");
}


/********************* INPUT FUNCTIONS *********************/

/*
 *   Function 'read_checkerboard_matrix' reads a matrix from
 *   a file. The first two elements of the file are integers
 *   whose values are the dimensions of the matrix ('m' rows
 *   and 'n' columns). What follows are 'm'*'n' values
 *   representing the matrix elements stored in row-major
 *   order.  This function allocates blocks of the matrix to
 *   the MPI processes.
 *
 *   The number of processes must be a square number.
 */
 
void read_checkerboard_matrix (
   char *s,              /* IN - File name */
   void ***subs,         /* OUT - 2D array */
   void **storage,       /* OUT - Array elements */
   MPI_Datatype dtype,   /* IN - Element type */
   int *m,               /* OUT - Array rows */
   int *n,               /* OUT - Array cols */
   MPI_Comm grid_comm)   /* IN - Communicator */
{
   void      *buffer;         /* File buffer */
   int        coords[2];      /* Coords of proc receiving
                                 next row of matrix */
   int        datum_size;     /* Bytes per elements */
   int        dest_id;        /* Rank of receiving proc */
   int        grid_coord[2];  /* Process coords */
   int        grid_id;        /* Process rank */
   int        grid_period[2]; /* Wraparound */
   int        grid_size[2];   /* Dimensions of grid */
   int        i, j, k;
   FILE      *infileptr;      /* Input file pointer */
   void      *laddr;          /* Used when proc 0 gets row */
   int        local_cols;     /* Matrix cols on this proc */
   int        local_rows;     /* Matrix rows on this proc */
   void     **lptr;           /* Pointer into 'subs' */
   int        p;              /* Number of processes */
   void      *raddr;          /* Address of first element
                                 to send */
   void      *rptr;           /* Pointer into 'storage' */
   MPI_Status status;         /* Results of read */

   MPI_Comm_rank (grid_comm, &grid_id);
   MPI_Comm_size (grid_comm, &p);
   datum_size = get_size (dtype);

   /* Process 0 opens file, gets number of rows and
      number of cols, and broadcasts this information
      to the other processes. */

   if (grid_id == 0) {
      infileptr = fopen (s, "r");
      if (infileptr == NULL) *m = 0;
      else {
         fread (m, sizeof(int), 1, infileptr);
         fread (n, sizeof(int), 1, infileptr);
      }
   }
   MPI_Bcast (m, 1, MPI_INT, 0, grid_comm);

   if (!(*m)) MPI_Abort (MPI_COMM_WORLD, OPEN_FILE_ERROR);

   MPI_Bcast (n, 1, MPI_INT, 0, grid_comm);

   /* Each process determines the size of the submatrix
      it is responsible for. */

   MPI_Cart_get (grid_comm, 2, grid_size, grid_period,
      grid_coord);
   local_rows = BLOCK_SIZE(grid_coord[0],grid_size[0],*m);
   local_cols = BLOCK_SIZE(grid_coord[1],grid_size[1],*n);

   /* Dynamically allocate two-dimensional matrix 'subs' */

   *storage = my_malloc (grid_id,
      local_rows * local_cols * datum_size);
   *subs = (void **) my_malloc (grid_id,local_rows*PTR_SIZE);
   lptr = (void *) *subs;
   rptr = (void *) *storage;
   for (i = 0; i < local_rows; i++) {
      *(lptr++) = (void *) rptr;
      rptr += local_cols * datum_size;
   }

   /* Grid process 0 reads in the matrix one row at a time
      and distributes each row among the MPI processes. */

   if (grid_id == 0)
      buffer = my_malloc (grid_id, *n * datum_size);

   /* For each row of processes in the process grid... */
   for (i = 0; i < grid_size[0]; i++) {
      coords[0] = i;

      /* For each matrix row controlled by this proc row...*/
      for (j = 0; j < BLOCK_SIZE(i,grid_size[0],*m); j++) {

         /* Read in a row of the matrix */

         if (grid_id == 0) {
            fread (buffer, datum_size, *n, infileptr);
         }

         /* Distribute it among process in the grid row */

         for (k = 0; k < grid_size[1]; k++) {
            coords[1] = k;

            /* Find address of first element to send */
            raddr = buffer +
               BLOCK_LOW(k,grid_size[1],*n) * datum_size;

            /* Determine the grid ID of the process getting
               the subrow */
            MPI_Cart_rank (grid_comm, coords, &dest_id);

            /* Process 0 is responsible for sending...*/
            if (grid_id == 0) {

               /* It is sending (copying) to itself */
               if (dest_id == 0) {
                  laddr = (*subs)[j];
                  memcpy (laddr, raddr,
                     local_cols * datum_size);

               /* It is sending to another process */
               } else {
                  MPI_Send (raddr,
                     BLOCK_SIZE(k,grid_size[1],*n), dtype,
                  dest_id, 0, grid_comm);
               }

            /* Process 'dest_id' is responsible for
               receiving... */
            } else if (grid_id == dest_id) {
               MPI_Recv ((*subs)[j], local_cols, dtype, 0,
                  0, grid_comm,&status);
            }
         }
      }
   }
   if (grid_id == 0) free (buffer);
}


/*
 *   Print a matrix distributed checkerboard fashion among
 *   the processes in a communicator.
 */

void print_checkerboard_matrix (
   void       **a,            /* IN -2D matrix */
   MPI_Datatype dtype,        /* IN -Matrix element type */
   int          m,            /* IN -Matrix rows */
   int          n,            /* IN -Matrix columns */
   MPI_Comm     grid_comm)    /* IN - Communicator */
{
   void      *buffer;         /* Room to hold 1 matrix row */
   int        coords[2];      /* Grid coords of process
                                 sending elements */
   int        datum_size;     /* Bytes per matrix element */
   int        els;            /* Elements received */
   int        grid_coords[2]; /* Coords of this process */
   int        grid_id;        /* Process rank in grid */
   int        grid_period[2]; /* Wraparound */
   int        grid_size[2];   /* Dims of process grid */
   int        i, j, k;
   void      *laddr;          /* Where to put subrow */
   int        local_cols;     /* Matrix cols on this proc */
   int        p;              /* Number of processes */
   int        src;            /* ID of proc with subrow */
   MPI_Status status;         /* Result of receive */

   MPI_Comm_rank (grid_comm, &grid_id);
   MPI_Comm_size (grid_comm, &p);
   datum_size = get_size (dtype);

   MPI_Cart_get (grid_comm, 2, grid_size, grid_period,
      grid_coords);
   local_cols = BLOCK_SIZE(grid_coords[1],grid_size[1],n);

   if (!grid_id)
      buffer = my_malloc (grid_id, n*datum_size);

   /* For each row of the process grid */
   for (i = 0; i < grid_size[0]; i++) {
      coords[0] = i;

      /* For each matrix row controlled by the process row */
      for (j = 0; j < BLOCK_SIZE(i,grid_size[0],m); j++) {

         /* Collect the matrix row on grid process 0 and
            print it */
         if (!grid_id) {
            for (k = 0; k < grid_size[1]; k++) {
               coords[1] = k;
               MPI_Cart_rank (grid_comm, coords, &src);
               els = BLOCK_SIZE(k,grid_size[1],n);
               laddr = buffer +
                  BLOCK_LOW(k,grid_size[1],n) * datum_size;
               if (src == 0) {
                  memcpy (laddr, a[j], els * datum_size);
               } else {
                  MPI_Recv(laddr, els, dtype, src, 0,
                     grid_comm, &status);
               }
            }
            print_subvector (buffer, dtype, n);
            putchar ('\n');
         } else if (grid_coords[0] == i) {
            MPI_Send (a[j], local_cols, dtype, 0, 0,
               grid_comm);
         }
      }
   }
   if (!grid_id) {
      free (buffer);
      putchar ('\n');
   }
}












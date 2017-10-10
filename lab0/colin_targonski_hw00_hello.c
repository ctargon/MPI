/* C Example */
#include <stdio.h>
#include <mpi.h>
int main (int argc, char *argv[])
{
 int rank; /* rank is your task id, staring with 0 */
 int size; /* size, is the number of tasks you */
 char name[MPI_MAX_PROCESSOR_NAME];
 int len;
 /* ran the program with */
 /* never make MPI calls before this and */
 /* never touch argc and argv before doing this */
 MPI_Init (&argc, &argv);
 /* get current MPI task number */
 MPI_Comm_rank (MPI_COMM_WORLD, &rank);
 /* get number of MPI tasks */
 MPI_Comm_size (MPI_COMM_WORLD, &size);
 
 MPI_Get_processor_name(name, &len);
 /* regular old printf */
 printf("Hello world from task %d of %d on %s\n", rank, size, name);
 /* don't make MPI calls after this */
 MPI_Finalize();
 return 0;
} 

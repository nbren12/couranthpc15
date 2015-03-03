#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mpi.h"


int main(int argc, char *argv[])
{
  if (argc != 2)
  {
      printf("Need at least one argument\n");
      exit(0);
  }
  
    
    
  int N = atoi(argv[1]);


  int megabyte = pow(2,20);
  int sum_n = 2 * megabyte / sizeof(int);

  void* sum_ptr = malloc(sum_n * sizeof(int));  // allocate memory
  

  MPI_Init(&argc, &argv);
  MPI_Status status;

  int rank, tag, world_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int dest = (rank + 1) % world_size;
  int src = ( rank == 0) ? world_size -1 : rank -1;
  
  /* printf("I am %d, recv from %d, and sending to %d\n", rank, src, dest); */
  
  
  tag = 0;
  
  int i;

  for (i = 0; i < N; i++) {
    if (rank == 0) {
      MPI_Send(sum_ptr, sum_n, MPI_INT, dest, tag, MPI_COMM_WORLD);
      MPI_Recv(sum_ptr, sum_n, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
    } else {
      MPI_Recv(sum_ptr, sum_n, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
      MPI_Send(sum_ptr, sum_n, MPI_INT, dest, tag, MPI_COMM_WORLD);
    }
  }

  MPI_Finalize();
  return 0;
}

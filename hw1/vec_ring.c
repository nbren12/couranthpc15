#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mpi.h"
#include "util.h"



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
  
  // Setup loop and timing
  double local_time;
  timestamp_type t1, t2;
  
  get_timestamp(&t1);
  
  
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

  // Get timing information
  get_timestamp(&t2);
  local_time = timestamp_diff_in_seconds(t1, t2);
  if (rank == 0){
    printf("Work done\n");
    printf("Total time = %f (ms)\n", 1000 * local_time);
    printf("Number of communication = %d\n", N * world_size);
    printf("Size of communication = %f (MB)\n", sum_n * sizeof(int) / pow(2,20) );
    printf("Average Bandwidth = %f(MB/s)\n",
           sum_n * sizeof(int) / pow(2,20) * N * world_size / local_time);
  }

  MPI_Finalize();
  return 0;
}

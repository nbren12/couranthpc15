#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"
#include "util.h"
#include "math.h"




int main(int argc, char *argv[])
{
  if (argc != 2)
  {
      printf("Need at least one argument\n");
      exit(0);
  }
  
    
    
  int N = atoi(argv[1]);
  int sum = 0; // Only effects proc with rank 0
  void* sum_ptr = (void*) &sum;
  

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
  
  int i;

  get_timestamp(&t1);

  for (i = 0; i < N; i++) {
    if (rank == 0) {
      sum += rank;
      MPI_Send(sum_ptr, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
      MPI_Recv(sum_ptr, 1, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
    } else {
      MPI_Recv(sum_ptr, 1, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
      sum += rank;
      MPI_Send(sum_ptr, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
    }
  }

  // Get timing information
  get_timestamp(&t2);
  local_time = timestamp_diff_in_seconds(t1, t2);

  if (rank == 0) {
    printf("Sum is %d\n", sum);
    printf("Total time = %f (ms)\n", 1000 * local_time);
    printf("Number of communications = %d\n", world_size * N);
    printf("Latency = %f (ns) \n", pow(10,9)*local_time / world_size / N);
  }
  
  
  MPI_Finalize();
  
  return 0;
}

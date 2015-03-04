#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"


#define STOP_ITER_RAT 10e-6
#define OMEGA_OVER_RELAXED 1.5


double calc_resid(int N, double h2, double* f, double *u){
  int i;
  double resid = 0.0;
  double ai;

  

  for (i = 1; i < N-1; i++) {
    ai = (- u[i+1] + 2 * u[i] - u[i-1])/h2 -f[i];
    resid += ai*ai;
  }
  
  return resid;
}

double calc_resid_global(int N, double h2, double* f, double *u){

  double resid_local, resid_global;
  
  
  resid_local = calc_resid(N, h2, f, u);
  
  MPI_Allreduce((void *) &resid_local, (void*) & resid_global, 1,
                MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  resid_global = sqrt(resid_global);

  return resid_global;
  
}


void jacobi_laplace(int N, double h2, double *f, double *u, double *uc){
  int i;

  for (i = 0; i < N; i++) {
    uc[i] = u[i];
  }


  for (i = 1; i < N-1; i++) {
    u[i] = (h2 * f[i] + uc[i-1] + uc[i+1])/2.0;
  }

}

void fill_ghost_cell(int rank, int world_size, MPI_Status * status,
                     int n_per_proc, double* u){
  if (rank > 0)
    MPI_Send((void* ) (u + 1), 1, MPI_DOUBLE, rank -1, 0, MPI_COMM_WORLD);

  if (rank < world_size - 1)
    MPI_Send((void* ) (u + n_per_proc - 2), 1, MPI_DOUBLE, rank +1, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    u[0] = 0.0;
  } else {
    MPI_Recv((void *) u, 1, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, status);
  }

  if (rank == world_size -1) {
    u[n_per_proc -1] = 0.0;
  } else {
    MPI_Recv((void *) (u+n_per_proc -1), 1, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, status);
    
  }
  
}
int main(int argc, char *argv[])
{

  int N = atoi(argv[1]);
  double h2 = 1.0/(N+1)/(N+1);
  double *u, *uc, *f;

  MPI_Init(&argc, &argv);
  MPI_Status status;

  int rank, tag, world_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if (N % world_size != 0 ){
    printf("Number of processors must be a factor of the number of gridpoints\n");
    MPI_Finalize();
    exit(-1);
  } 

  // Number of points in each array (+2 for boundaries)
  const int n_per_proc = N / world_size + 2;

  u = (double *) malloc(n_per_proc*sizeof (double));
  uc = (double *) malloc(n_per_proc*sizeof (double));

  f = (double *) malloc(n_per_proc*sizeof (double));

  // initialize f and u
  int i;
  for (i = 0; i < n_per_proc-1; i++) {
    f[i] = 1.0;
    u[i] = 0.0;
  }

  // Begin iterations
  double resid_init;
  double resid_global;
  
  // Calculate Residual
  resid_global = calc_resid_global(n_per_proc, h2, f, u);
  resid_init = resid_global;
  

  
  
  
  while (resid_global / resid_init > STOP_ITER_RAT){
    
    if (rank == 0)
        printf("Resid is %f\n", resid_global);
    fill_ghost_cell(rank, world_size, &status, n_per_proc, u);
    jacobi_laplace(n_per_proc, h2, f, u, uc);

    // Calculate residual
    fill_ghost_cell(rank, world_size, &status, n_per_proc, u);
    resid_global = calc_resid_global(n_per_proc, h2, f, u);

  }
  if (rank == 0)
    printf("Resid is %f\n", resid_global);

  // deallocate
  free(f);
  free(u);
  free(uc);
  
  MPI_Finalize();
  return 0;
}

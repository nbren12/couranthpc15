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
  
  return sqrt(resid);
}


void jacobi_laplace(int N, double h2, double *f, double *u, double *uc){
  int i;

  for (i = 1; i < N-1; i++) {
    uc[i] = u[i];
  }


  for (i = 1; i < N-1; i++) {
    u[i] = (h2 * f[i] + uc[i-1] + uc[i+1])/2.0;
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
  double resid_init, resid_cur;
  resid_init = calc_resid(n_per_proc, h2, f, u);
  resid_cur = resid_init;
  printf("%f\n", resid_init);
  
  
  while (resid_cur / resid_init > STOP_ITER_RAT){
    
    u[0] = 0.0;
    u[n_per_proc - 1] = 0.0;
    
    jacobi_laplace(n_per_proc, h2, f, u, uc);

    resid_cur = calc_resid(n_per_proc, h2, f, u);
    printf("Resid is %f\n", resid_cur );
  }

  // deallocate
  free(f);
  free(u);
  free(uc);
  
  MPI_Finalize();
  return 0;
}

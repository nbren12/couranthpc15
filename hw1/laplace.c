#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STOP_ITER_RAT 10e-6
#define OMEGA_OVER_RELAXED 1.5


double calc_resid(int N, double* f, double *u){
  int i;
  double resid = 0.0;
  double ai;

  double h2 = 1.0/(N+1)/(N+1);
  
  ai  =  (2 * u[0] - u[1])/h2 -f[0];
  resid += ai*ai;

  ai =  (2 * u[N-1] - u[N-2])/h2 -f[N-1];
  resid += ai*ai;

  for (i = 1; i < N-1; i++) {
    ai = (- u[i+1] + 2 * u[i] - u[i-1])/h2 -f[i];
    resid += ai*ai;
  }
  
  return sqrt(resid);
}


void jacobi_laplace(int N, double *f, double *u, double *uc){
  int i;
  double h2 = 1.0/(N+1)/(N+1);

  for (i = 0; i < N; i++) {
    uc[i] = u[i];
  }

  u[0] =  (h2 * f[0] + uc[1])/2.0;

  for (i = 1; i < N-1; i++) {
    u[i] = (h2 * f[i] + uc[i-1] + uc[i+1])/2.0;
  }

  u[N-1] = (h2* f[N-1] + uc[N-2])/2.0;
}



int main(int argc, char *argv[])
{

  int N = atoi(argv[1]);

  double *u, *uc, *f;

  // allocate arrays
  u = (double *) malloc(N*sizeof (double));
  uc = (double *) malloc(N*sizeof (double));

  f = (double *) malloc(N*sizeof (double));

  // initialize f and u
  int i;
  for (i = 0; i < N; i++) {
    f[i] = 1.0;
    u[i] = 0.0;
  }

  // Begin iterations
  double resid_init, resid_cur;
  resid_init = calc_resid(N, f, u);
  resid_cur = resid_init;
  
  printf("%f\n", resid_init);
  while (resid_cur / resid_init > STOP_ITER_RAT){
    
    jacobi_laplace(N, f, u, uc);

    resid_cur = calc_resid(N, f, u);
    printf("Resid is %f\n", resid_cur );
  }

  // deallocate
  free(f);
  free(u);
  free(uc);
  
  return 0;
}

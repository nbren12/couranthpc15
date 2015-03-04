NP=4
N=10

runjac: jacobi-mpi
	mpirun -np 4 ./jacobi-mpi 1000 1

all: int_ring jacobi-mpi

.PHONY: run
run: int_ring
	mpirun -np $(NP) ./int_ring $(N)

int_ring : int_ring.c
	mpicc -o $@ $^

vec_ring : vec_ring.c
	mpicc -o $@ $^

jacobi-serial: jacobi-serial.c
	clang -g -lm -o $@ jacobi-serial.c

laplace: laplace.c
	clang -g -lm -o $@ $^

jacobi-mpi: jacobi-mpi.c
	mpicc -lm -o $@ $^

NP=4
N=10

all: vec_ring int_ring jacobi-mpi

.PHONY: run runjac
run: int_ring
	mpirun -np $(NP) ./int_ring $(N)

runjac: jacobi-mpi
	mpirun -np 4 ./jacobi-mpi 1000 1

int_ring : int_ring.c
	mpicc -o $@ $^

vec_ring : vec_ring.c
	mpicc -o $@ $^

jacobi-serial: jacobi-serial.c
	clang -lm -o $@ jacobi-serial.c

laplace: laplace.c
	clang -g -lm -o $@ $^

jacobi-mpi: jacobi-mpi.c
	mpicc -o $@ $^

clean:
	rm -f jacobi-mpi
	rm -f laplace jacobi-serial vec_ring int_ring 

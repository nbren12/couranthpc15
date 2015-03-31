NP=4
N=10

CC=mpicc
CFLAGS= -lm

SRC = vec_ring.c int_ring.c jacobi-mpi.c 
EXE = $(SRC:.c=)

# Parallel codes
all: $(EXE)


# Serial codes
jacobi-serial: jacobi-serial.c
	clang -lm -o $@ jacobi-serial.c

laplace: laplace.c
	clang -g -lm -o $@ $^

# Phone targets
.PHONY: run runjac clean
run: int_ring
	mpirun -np $(NP) ./int_ring $(N)

runjac: jacobi-mpi
	mpirun -np 4 ./jacobi-mpi 1000 1

clean:
	rm -f jacobi-mpi
	rm -f laplace jacobi-serial vec_ring int_ring 

NP=4
N=10

.PHONY: run
run: int_ring
	mpirun -np $(NP) ./int_ring $(N)

int_ring : int_ring.c
	mpicc -o $@ $^

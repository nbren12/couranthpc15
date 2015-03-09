#!/bin/sh
#PBS -o /home/ndb245/int_ring.job.output
#PBS -l nodes=4:ppn=12,walltime=1000

mpirun -np 48 /home/ndb245/tmp/couranthpc15/hw1/int_ring 1000

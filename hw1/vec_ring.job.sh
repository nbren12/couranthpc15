#!/bin/sh
#PBS -o /home/ndb245/vec_ring.job.output
#PBS -l nodes=4:ppn=12,walltime=1000

mpiexec -n 48 /home/ndb245/tmp/couranthpc15/hw1/vec_ring 1000

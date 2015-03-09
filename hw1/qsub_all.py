import os
from subprocess import call
import tempfile


d = tempfile.mkdtemp(prefix='.tmp.', dir=os.getcwd())
os.chdir(d)

for np in [1, 2, 4, 8, 12, 24, 48]:
    if np < 12:
        nodes = 1
    else:
        nodes = int(np / 12)

    subfile = '{np}.job.sh'.format(np=np)

    with open(subfile, 'w') as f:
        f.write('#!/bin/sh\n')
        f.write('#PBS -l nodes={nodes}:ppn=12\n'.format(nodes=nodes))
        f.write('#PBS -o /home/ndb245/job.output/{np}.output\n'.format(np=np))
        f.write('mpirun -np {np}  /home/ndb245/tmp/couranthpc15/hw1/jacobi-mpi 48000 1000\n'.format(**locals()))

    os.system('qsub %s'%subfile)

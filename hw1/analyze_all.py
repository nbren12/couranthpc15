import sh
import re
import matplotlib.pyplot as plt
import pandas as pd

def runtime(np):
    outfile = '/home/ndb245/job.output/{np}.output'.format(np=np)
    str = sh.tail('-n', 1, outfile).strip()
    time = float(re.search(r'[\d\.]+', str).group(0))
    return time

output = [(np, runtime(np) )for np in [1, 2, 4, 8, 12, 48]]

np, runtime =  zip(*output)
series = pd.Series(runtime, index=np)
series.plot(kind='bar')

plt.xlabel('Number of processors')
plt.ylabel('Run time')
plt.savefig('jacobi-mpi-scaling.pdf')

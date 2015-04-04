import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import cumfreq
import scikits.statsmodels as sm
from matplotlib import pyplot
from matplotlib.gridspec import GridSpec

 
def plot_func_cpu(filename,index_,marker):
    f = open(filename)
    a1 = f.readlines()
    f.close()
    runtimes = [float(val) for val in a1]
    runtimes = np.array(runtimes) # my array of numbers
    sorted_data = np.sort(runtimes)
    max_val = np.max(sorted_data)
    min_val = np.min(sorted_data)
    print max_val, "," , min_val
    hi,label_ = np.histogram(sorted_data, bins=np.arange(0,max_val + 0.01,0.01))
    csum = np.cumsum(hi) 
    line_id = 'Trace ' + index_
    plt.plot(label_, np.insert(csum, 0, 0),label=line_id,marker=marker,markersize=4)
   
def plot_func_mem(filename,index_,marker):
    f = open(filename)
    a1 = f.readlines()
    f.close()
    runtimes = [float(val) for val in a1]
    runtimes = np.array(runtimes) # my array of numbers
    sorted_data = np.sort(runtimes)
    max_val = np.max(sorted_data)
    min_val = np.min(sorted_data)
    print max_val, "," , min_val
    hi,label_ = np.histogram(sorted_data, bins=np.arange(0,max_val + 0.0005,0.0005))
    csum = np.cumsum(hi) 
    line_id = 'Trace ' + index_
    plt.plot(label_, np.insert(csum, 0, 0),label=line_id,marker=marker,markersize=4)


plot_func_cpu('/Users/anirudhnair/code/GeoSched/workloads/google/5_1cpu','A','o')
plot_func_cpu('/Users/anirudhnair/code/GeoSched/workloads/google/5_2cpu','B','^')
plot_func_cpu('/Users/anirudhnair/code/GeoSched/workloads/google/5_3cpu','C','+')
plot_func_cpu('/Users/anirudhnair/code/GeoSched/workloads/google/5_4cpu','D','s')
plot_func_cpu('/Users/anirudhnair/code/GeoSched/workloads/google/5_5cpu','E','x')
plt.ylabel(r'Job Count')
plt.xlabel(r'CPU resources')
plt.xscale('log',basex=2)
legend = plt.legend(loc='lower right', shadow=True)
plt.savefig('resource_cpu.jpg', format='jpg', dpi=600)
plt.clf()

plot_func_mem('/Users/anirudhnair/code/GeoSched/workloads/google/5_1mem','A','o')
plot_func_mem('/Users/anirudhnair/code/GeoSched/workloads/google/5_2mem','B','^')
plot_func_mem('/Users/anirudhnair/code/GeoSched/workloads/google/5_3mem','C','+')
plot_func_mem('/Users/anirudhnair/code/GeoSched/workloads/google/5_4mem','D','s')
plot_func_mem('/Users/anirudhnair/code/GeoSched/workloads/google/5_5mem','E','x')
plt.ylabel(r'Job Count')
plt.xlabel(r'Memory resources')
plt.xscale('log',basex=2)
#plt.yscale('log',basey=2)
legend = plt.legend(loc='lower right', shadow=True)

plt.savefig('resource_mem.jpg', format='jpg', dpi=600)
plt.clf()

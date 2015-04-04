import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import cumfreq
import scikits.statsmodels as sm
from matplotlib import pyplot
 
def plot_func(filename,index_,marker):
    f = open(filename)
    a1 = f.readlines()
    f.close()
    runtimes = [int(val) for val in a1]
    runtimes = np.array(runtimes) # my array of numbers
    sorted_data = np.sort(runtimes)
    sorted_data = np.divide(sorted_data,1000000)
    max_time = np.max(sorted_data) 
    max_time = max_time + (100 - max_time%100)
    hi,label_ = np.histogram(sorted_data, bins=range(0,max_time + 100,100))
    csum = np.cumsum(hi) 
    line_id = 'Trace ' + index_
    plt.plot(label_, np.insert(csum, 0, 0),label=line_id,marker=marker,markersize=4)
   


plot_func('/Users/anirudhnair/code/GeoSched/workloads/google/5_1runtime.csv','A','o')
plot_func('/Users/anirudhnair/code/GeoSched/workloads/google/5_2runtime.csv','B','^')
plot_func('/Users/anirudhnair/code/GeoSched/workloads/google/5_3runtime.csv','C','+')
plot_func('/Users/anirudhnair/code/GeoSched/workloads/google/5_4runtime.csv','D','s')
plot_func('/Users/anirudhnair/code/GeoSched/workloads/google/5_5runtime.csv','E','x')

plt.xscale('log',basex=2)
#plt.yscale('log',basey=2)
legend = plt.legend(loc='lower right', shadow=True)
plt.ylim([20000,80000])
plt.ylabel(r'Job Count')
plt.xlabel(r'Job duration(secs)')
plt.savefig('runtime.jpg', format='jpg', dpi=600)
plt.clf()


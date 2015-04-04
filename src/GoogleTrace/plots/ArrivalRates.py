import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import os

f = open("/Users/anirudhnair/code/GeoSched/workloads/google/5_1arrival.csv")
a1 = f.readlines()
f.close()
f = open("/Users/anirudhnair/code/GeoSched/workloads/google/5_2arrival.csv")
a2 = f.readlines()
f.close()
f = open("/Users/anirudhnair/code/GeoSched/workloads/google/5_3arrival.csv")
a3 = f.readlines()
f.close()
f = open("/Users/anirudhnair/code/GeoSched/workloads/google/5_4arrival.csv")
a4 = f.readlines()
f.close()
f = open("/Users/anirudhnair/code/GeoSched/workloads/google/5_5arrival.csv")
a5 = f.readlines()
f.close()

plt.rc('text', usetex=False)
plt.rc('font', family='serif')

#xLabel_ = [0,15,30,45,60,75,90,105,120]
#plt.xticks(range(len(xLabel_)), xLabel_, size='medium')

#plt.scatter(range(len(a1)),a1)
plt.plot(range(len(a1)), a1, linestyle='-' ,marker='o', markersize=4,label='Trace A')

#plt.scatter(range(len(a2)),a2)
plt.plot(range(len(a2)), a2, linestyle='-' , marker='^', markersize=4,label='Trace B')

#plt.scatter(range(len(a3)),a3)
plt.plot(range(len(a3)), a3, linestyle='-' , marker='+', markersize=4,label='Trace C')

#plt.scatter(range(len(a4)),a4)
plt.plot(range(len(a4)), a4, linestyle='-' , marker='s', markersize=4,label='Trace D')

#plt.scatter(range(len(a5)),a5)
plt.plot(range(len(a5)), a5,  linestyle='-' , marker='x', markersize=4,label='Trace E')

plt.ylim([200,1400])
legend = plt.legend(loc='upper right', shadow=True)

plt.ylabel(r'Arriving Job Count')
plt.xlabel(r'Hours')


plt.savefig('arrivalrate.jpg', format='jpg', dpi=600)
plt.clf()
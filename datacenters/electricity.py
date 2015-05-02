import os
import traceback
import sys
import numpy
import matplotlib.pyplot as plt 
from pylab import *
import numpy as np
import calendar


cwd=os.getcwd()

address="/Users/harshitdokania/Desktop/cs525/geosched/datacenters/electricity"




def ConvertCSV(filename, address):
 
 weeklyTemp=[]
 count=1;
 
 with open(os.path.join(address, filename), 'r') as txt_file:
     avg=0;
     for line in txt_file:
       myData=line.split(",")  
       t=myData[4]
       print t 
       print count
       if count % 168==1 and count != 1:
         weeklyTemp.append(avg/168)
         avg=float(t)
         count=count+1
       else:
         avg=avg + float(t)
         count=count+1
     rem = count % 168 -1
     weeklyTemp.append(avg/rem)
 return weeklyTemp 
Chil_avg=ConvertCSV("chile.csv", address)    
print len(Chil_avg)
fin_avg=ConvertCSV("finland.csv", address)    
print len(fin_avg)
iowa_avg=ConvertCSV("iowa.csv", address)    
ore_avg=ConvertCSV("oregon.csv", address)    
sing_avg=ConvertCSV("singapore.csv", address)    

# evenly sampled time at 200ms intervals
weeks=[]
for i in range(53):
  weeks.append(i)
 


# red dashes, blue squares and green triangles
x = np.arange(0, 53, 4.5) 
my_xticks = ['Jan', 'Feb', 'Mar','Apr','May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
plt.xticks(x, my_xticks)
plt.plot(weeks, Chil_avg, 'g-^', label='Chile')
plt.plot(weeks, fin_avg, 'b-o', label='Finland')
plt.plot(weeks, iowa_avg, '-rs', label='Iowa')
plt.plot(weeks, ore_avg, 'm-*', label='Oregon')
plt.plot(weeks, sing_avg, 'y-o', label='Singapore')
plt.legend( loc='upper right', numpoints = 1 )
plt.xlabel('Months', fontsize=18)
plt.ylabel('Weekly average Electricity Price', fontsize=18)
#plt.plot(x,avg_IRE,'+', label="Ireland")
#plt.plot(weeks, Chil_avg, 'r--', t, t**2, 'bs', t, t**3, 'g^')
plt.show() 

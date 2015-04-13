import os
import traceback
import sys
import numpy
import matplotlib.pyplot as plt 
from pylab import *
import numpy as np
import calendar


cwd=os.getcwd()

address = cwd+"/"
IRE=address + "/IRE"
IRE="/Users/harshitdokania/Desktop/cs525/geosched/data_centers/IRE"
IRE_file="3982496643522dat.txt"
NC_file="8845366643370dat.txt"
NC=address + "/North_Carolina"
Berk_file="5475956643413dat.txt"
Berk=address + "/berkeley"
Chile_file="7361266643506dat.txt"
Chile=address+"/chile"
Sing_file="5146466643516dat.txt"
Sing=address + "/singapore"



def ConvertCSV(filename, address, output_file):
 count=0
 date=0
 hr=0
 old_date_0=0
 old_date_1=1
 avg=[]
 sum1=0
 YEAR=''
 MONTH=''
 DAY=''
 HOUR=''
 with open(os.path.join(address, filename), 'r') as txt_file:
     for line in txt_file:
       myData=line.split() 
       count=count+1
       if count==1:
         continue
       DATE=myData[2]
       date_0=DATE[6]
       date_1=DATE[7]
       for i in range(0,12):
         if i<4:
           YEAR=YEAR+DATE[i]
         elif i>=4 and i<6:
           MONTH=MONTH+DATE[i]
         elif i>=6 and i<8:
           DAY=DAY + DATE[i]
         elif i>=8 and i<12:
           HOUR=HOUR + DATE[i]
       TEMP=myData[21]
    #date_0=DATE[6]
       print DATE
       formatted=YEAR +',' + MONTH + ',' + DAY + ',' + HOUR + ',' + TEMP 
       outfile = open(output_file,'a')
       print >> outfile,formatted
       outfile.close()
       YEAR=''
       MONTH=''
       DAY=''
       HOUR=''
       if count==2:
         old_date_0=date_0  
         old_date_1=date_1 
       if date_0==old_date_0 and date_1==old_date_1 :
         pass
       else:
         old_date_0=date_0  
         old_date_1=date_1 

ConvertCSV(Chile_file,Chile, "Chile_Temp.csv")      
ConvertCSV(IRE_file,IRE, "IRE_Temp.csv")      
ConvertCSV(NC_file,NC, "NC_Temp.csv")      
ConvertCSV(Berk_file,Berk, "Berk_Temp.csv")      
ConvertCSV(Sing_file,Sing, "Sing_Temp.csv")      
'''
x = np.arange(0, 365, 30) 
#my_xticks = ['Mar','Apr','May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec', 'Jan', 'Feb']
#plt.xticks(x, my_xticks)
avg_IRE=Giveaverage(IRE_file,IRE)      
avg_Chile=Giveaverage(Chile_file,Chile)      
avg_Berk=Giveaverage(Berk_file,Berk)      
avg_Sing=Giveaverage(Sing_file,Sing)      
avg_NC=Giveaverage(NC_file,NC)      
#plt.plot(x,avg_IRE,'+', label="Ireland")
plt.plot(avg_IRE,'+', label="Ireland")
legend(framealpha=0.5)
#plt.plot(x,avg_Chile,'^',label="Chile")
plt.plot(avg_Chile,'^',label="Chile")
legend(framealpha=0.5)
#plt.plot(x,avg_Berk,'s',label="Berkeley")
plt.plot(avg_Berk,'s',label="Berkeley")
legend(framealpha=0.5)
#plt.plot(x,avg_Sing,'*',label="Singapore")
plt.plot(avg_Sing,'*',label="Singapore")
legend(framealpha=0.5)
#plt.plot(x,avg_NC, 'o',label="North Carolina") 
plt.plot(avg_NC, 'o',label="North Carolina") 
#xticks( arange(12), calendar.month_name[1:13])
xticks([31,61,92,122,153,184,214,245,275,306,337,365], ["Mar","Apr","May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", "Jan", "Feb"])
legend(framealpha=0.5)
plt.xlabel('Months in a year')
plt.ylabel('Average Temp $^\circ$ F')

plt.grid()
plt.show()    
'''

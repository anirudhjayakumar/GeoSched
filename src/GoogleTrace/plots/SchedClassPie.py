"""
Make a pie chart - see
http://matplotlib.sf.net/matplotlib.pylab.html#-pie for the docstring.

This example shows a basic pie chart with labels optional features,
like autolabeling the percentage, offsetting a slice with "explode",
adding a shadow, and changing the starting angle.

"""
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
from matplotlib import font_manager as fm
# make a square figure and axes
#plt.figure(1, figsize=(5,2.5))    
#ax = plt.axes([0.1, 0.1, 0.8, 0.8])

# The slices will be ordered and plotted counter-clockwise.
labels = r"C0", r"C1", r"C2", r"C3"

mpl.rcParams['font.size'] = 9.0
mpl.rcParams['lines.linewidth'] = 0.1
color_ = ['r', 'y', 'g', 'b']
plt.rc('font', family='serif')
explode = (0, 0, 0, 0)
the_grid = GridSpec(1, 5)

plt.axis('equal')
fracs = [30.43,45.36,24.19,0.02]
plt.subplot(the_grid[0, 0], aspect=1)
patches, texts, autotexts = plt.pie(fracs, explode=explode,labels=labels,labeldistance=1.05,
                autopct="%1.2f%%", shadow=False, startangle=0, colors=color_,radius=1.2)
for w in patches:
    w.set_linewidth( 0 )
proptease = fm.FontProperties()
proptease.set_size('x-small')
plt.setp(autotexts, fontproperties=proptease)
plt.setp(texts, fontproperties=proptease)
plt.title(r"Trace A")
fracs = [30.97,44.44,24.57,0.02]
plt.subplot(the_grid[0, 1], aspect=1)
patches, texts, autotexts = plt.pie(fracs, explode=explode,labels=labels,labeldistance=1.05,
                autopct="%1.2f%%", shadow=False, startangle=0, colors=color_,radius=1.2)
for w in patches:
    w.set_linewidth( 0 )
proptease = fm.FontProperties()
proptease.set_size('x-small')
plt.setp(autotexts, fontproperties=proptease)
plt.setp(texts, fontproperties=proptease)

plt.title(r"Trace B")
fracs = [31.93,43.09,24.96,0.03]
plt.subplot(the_grid[0, 2], aspect=1)
patches, texts, autotexts = plt.pie(fracs, explode=explode,labels=labels,labeldistance=1.05,
                autopct="%1.2f%%", shadow=False, startangle=0, colors=color_,radius=1.2)
for w in patches:
    w.set_linewidth( 0 )
proptease = fm.FontProperties()
proptease.set_size('x-small')
plt.setp(autotexts, fontproperties=proptease)
plt.setp(texts, fontproperties=proptease)
plt.title(r"Trace C")

fracs = [27.57,39.70,32.71,0.01]
plt.subplot(the_grid[0, 3], aspect=1)
patches, texts, autotexts = plt.pie(fracs, explode=explode,labels=labels,labeldistance=1.05,
                autopct="%1.2f%%", shadow=False, startangle=0, colors=color_,radius=1.2)
for w in patches:
    w.set_linewidth( 0 )
proptease = fm.FontProperties()
proptease.set_size('x-small')
plt.setp(autotexts, fontproperties=proptease)
plt.setp(texts, fontproperties=proptease)
plt.title(r"Trace D")

fracs = [28.62,46.96,24.41,0.01]
plt.subplot(the_grid[0, 4], aspect=1)
patches, texts, autotexts = plt.pie(fracs, explode=explode,labels=labels,labeldistance=1.05,
                autopct="%1.2f%%", shadow=False, startangle=0, colors=color_,radius=1.2)
for w in patches:
    w.set_linewidth( 0 )
proptease = fm.FontProperties()
proptease.set_size('x-small')
plt.setp(autotexts, fontproperties=proptease)
plt.setp(texts, fontproperties=proptease)
plt.title(r"Trace E")

plt.savefig('pie.jpg', format='jpg', dpi=600)
plt.clf()
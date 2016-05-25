import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import spline


fobja = open("/portal/ekpcms6/home/kfloeh/thanalysis/datacards/kappa_f_fit_c/ML.txt")
fobjb = open("/portal/ekpcms6/home/kfloeh/thanalysis/datacards/kappa_f_fit_f/ML.txt")
fobjc = open("/portal/ekpcms6/home/kfloeh/thanalysis/datacards/kappa_f_fit/ML.txt")
texta=[]
textb=[]
textc=[]
dataa=[0]*17
datab=[0]*17
datac=[0]*17
#kappa=[-1.0,-3.0,-2.0,-1.5,-1.25,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1.0,1.25,1.5,2.0,3.0]
kappa=[-3.0,-2.0,-1.5,-1.25,-1.0,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1.0,1.25,1.5,2.0,3.0]
for line in fobja:
	texta.append(line)
for i in range(0,len(texta)/2):
	words = texta[2*i+1].split()
	dataa[i] = -float(words[3])

datatempa=dataa[0]

for i in range(1,5):
	dataa[i-1]=dataa[i]

dataa[4]=datatempa
datab[4]=datatempb
datac[4]=datatempc

print dataa
print datab
print datac

plt.axis([-3.1, 3.1, 130, 200])
xnew = np.linspace(-3.0,3.0,300)
power_smooth = spline(kappa,dataa,xnew)
plt.plot(xnew,power_smooth,'r')
#power_smooth = spline(kappa,datab,xnew)
#plt.plot(xnew,power_smooth, 'b')
power_smooth = spline(kappa,datac,xnew)
plt.plot(xnew,power_smooth, 'g')
plt.grid(color='grey', linestyle=':', linewidth=1)
plt.plot(kappa, dataa, 'ro', label="central")
plt.plot(kappa, datab, 'bo', label="forward")
plt.plot(kappa, datac, 'go', label="total")
plt.xlabel('kappa_f')
plt.ylabel('nll S+B')
ax = plt.subplot(111)
box = ax.get_position()
ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))
#legend=plt.legend(loc=(0.8,0.75),fontsize=22)
plt.savefig('plot.pdf')

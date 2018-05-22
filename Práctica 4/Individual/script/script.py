
#!/usr/bin/env python

import os
import subprocess
import csv
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from numpy import array
import math

N=10

def f(x, a, b, c):
    return a*pow(x,2)+b*x+c

def data_generator(command_line, begin, n_steps, step):
    xdata=[]
    ydata=[]
    n=begin

    for i in range(n_steps):
        n+=step
        output = subprocess.check_output(command_line+ " " + str(n), shell=True)

        xdata.append(n)
        ydata.append(float(output.split()[1]))

    return [xdata, ydata]


os.system("g++ -O2 ./src/main1.cpp -o ./bin/main1;")
os.system("g++ -O2 ./src/main2.cpp -o ./bin/main2;")
os.system("g++ -O2 ./src/main3.cpp -o ./bin/main3;")

metodo1=data_generator("./bin/main1", 1, N, 1)
metodo2=data_generator("bin/main2", 1, N, 1)
metodo3=data_generator("bin/main3", 1, N, 1)

ns=array(metodo3[0])

metodo1=array(metodo1[1])
metodo2=array(metodo2[1])
metodo3=array(metodo3[1])

popt, pcov = curve_fit(f, ns, metodo1)
popt1, pcov1 = curve_fit(f, ns, metodo2)
popt2, pcov2 = curve_fit(f, ns, metodo3)

label='a=%5.9f b=%5.9f c=%5.9f'

plt.figure()
plt.plot(ns, f( ns, *popt), '-b', label='fit: ' + label % tuple(popt))
plt.scatter(ns, metodo1)
plt.legend()
plt.savefig("./graphics/metodo1.png")

plt.figure()
plt.plot(ns, f( ns, *popt1), '-r', label='fit: ' + label % tuple(popt1))
plt.scatter(ns, metodo2)
plt.legend()
plt.savefig("./graphics/metodo2.png")

plt.figure()
plt.plot(ns, f( ns, *popt2), '-y', label='fit: ' + label % tuple(popt2))
plt.scatter(ns, metodo3)
plt.legend()
plt.savefig("./graphics/metodo3.png")

plt.figure()
plt.plot(ns, f( ns, *popt), label='(1) fit: ' + label % tuple(popt))
plt.plot(ns, f( ns, *popt1), label='(2) fit: ' + label % tuple(popt1))
plt.plot(ns, f( ns, *popt2), label='(3) fit: ' + label % tuple(popt2))
plt.scatter(ns, metodo1)
plt.scatter(ns, metodo2)
plt.scatter(ns, metodo3)
plt.legend()
plt.savefig("./graphics/juntas.png")

#!/usr/bin/env python

import os
import subprocess
import csv
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from numpy import array
from tabulate import tabulate
import math

#====================FUNCIONES AUXILIARES====================

def write_csv(filename, data):
    file = open(str(filename), 'w')
    with file:
        writer = csv.writer(file)
        writer.writerows(data)

def read_csv(filepath):
    with open(filepath) as csvfile:
        readCSV = csv.reader(csvfile, delimiter=',')
        data = [ x for x in readCSV ]
        return data

def data_generator(directory, begin, n_steps, step):
    xdata=[]
    ydata=[]
    n=begin

    for i in range(n_steps):
        n+=step
        output = subprocess.check_output(directory+ " " + str(n), shell=True)

        xdata.append(n)
        ydata.append(float(output.split()[1]))

    return [xdata, ydata]

#====================FUNCIONES DE EFICIENCIA====================

def lineal(x, a, b):
    return a*x+b

def log(x, a, b):
    return a*np.log(x)+b

#====================COMPILACION====================

def compile(filename):
    os.system("g++ -O2 ./source/"+filename+".cpp -o ./bin/"+filename+" -std=gnu++0x")


#====================GENERACION====================

n=30
dN=1000

compile("repetidos")
compile("bruto")
compile("sin_repetir")  

a=data_generator("./bin/bruto", 0, n, dN)
b=data_generator("./bin/sin_repetir", 0, n, dN)
c=data_generator("./bin/repetidos", 0, n, dN)

x_b, y_b = array(a[0]), array(a[1])
x_sr, y_sr = array(b[0]), array(b[1])
x_r, y_r = array(c[0]), array(c[1])

popt_b, pcov_b = curve_fit(lineal, x_b, y_b)
popt_sr, pcov_sr = curve_fit(log, x_sr, y_sr)
popt_r, pcov_r = curve_fit(lineal, x_r, y_r)


# plt.figure()
# plt.plot(x_b, lineal( x_b, *popt_b), label='Bruto: a=%5.9f, b=%5.9f' % tuple(popt_b))
# plt.legend()
# plt.savefig('./graphics/buto.png')


# plt.figure()
# plt.plot(x_sr, log( x_sr, *popt_sr), label='Sin repetir: a=%5.9f, b=%5.9f' % tuple(popt_sr))
# plt.legend()
# plt.savefig('./graphics/sin_repetir.png')


# plt.figure()
# plt.plot(x_r, log( x_r, *popt_r), label='Repetidos: a=%5.9f, b=%5.9f' % tuple(popt_r))
# plt.legend()
# plt.scatter(x_r, y_r)
# plt.savefig('./graphics/repetidos.png')


# plt.figure()
# plt.plot(x_b, lineal( x_b, *popt_b),'r-', label='Bruto: a=%5.9f, b=%5.9f' % tuple(popt_b))
# plt.plot(x_sr, log( x_sr, *popt_sr), 'g-', label='Divide y Venceras: a=%5.9f, b=%5.9f' % tuple(popt_sr))
# plt.legend()
# plt.savefig('./graphics/juntas.png')


# os.system("rm ./bin/*")


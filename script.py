#!/usr/bin/env python

import os
import subprocess
import csv
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from numpy import array
from math import log

#====================FUNCIONES AUXILIARES====================

def write_csv(filename, data):
    file = open(str(filename), 'w')
    with file:
        writer = csv.writer(file)
        writer.writerows(data)

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

#====================FUNCIONES DE EFICIENCIA====================

def cuadratic(x, a, b, c):
    return a * x**2 + b * x + c
def n_log_n(x, a, b, c):
    return a*log(b*x)+c
def cubic(x, a, b, c, d):
    return a*x*x*x+b*x**2+c*x+d
def two_raised_to_the_n(x, a, b, c):
    return a*pow(2, b*x)+c

#====================COMPILACION====================

def compile(filename):
    os.system("g++ ./source/"+filename+".cpp -o ./bin/"+filename+" -std=gnu++0x")
    os.system("g++ ./source/"+filename+".cpp -o ./bin/"+filename+"_O1 -std=gnu++0x")
    os.system("g++ ./source/"+filename+".cpp -o ./bin/"+filename+"_O2 -std=gnu++0x")

#====================GENERACION DE GRAFICOS====================

def cuadratic_functions(filename, n_steps, n, f):
    
    compile(filename)

    data = data_generator("/home/antonio/Algoritmica/bin/" + filename, 0, n_steps, n)
    w = [ [ data[0][i], data[1][i] ] for i in range(n_steps) ]
    write_csv("./data/"+ filename + "/" + filename + ".csv", w)

    data_O1 = data_generator("/home/antonio/Algoritmica/bin/" + filename + "_O1 ", 0, n_steps, n)
    w = [ [ data_O1[0][i], data_O1[1][i] ] for i in range(n_steps) ]
    write_csv("./data/"+ filename + "/" + filename + "_O1.csv", w)

    data_O2 = data_generator("/home/antonio/Algoritmica/bin/" + filename + "_O2 ", 0, n_steps, n)
    w = [ [ data_O2[0][i], data_O2[1][i] ] for i in range(n_steps) ]
    write_csv("./data/"+ filename + "/" + filename + "_O2.csv", w)


    xdata = array(data[0])
    ydata = array(data[1])
    xdata_O1 = array(data_O1[0])
    ydata_O1 = array(data_O1[1])
    xdata_O2 = array(data_O2[0])
    ydata_O2 = array(data_O2[1])


    popt, pcov = curve_fit(f, xdata, ydata)
    popt_O1, pcov_01 = curve_fit(f, xdata_O1, ydata_O1)
    popt_O2, pcov_O2 = curve_fit(f, xdata_O2, ydata_O2)


    plt.figure()
    plt.plot(xdata, f( xdata, *popt), label='fit: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt))
    plt.legend()
    plt.savefig("./graficos/" + filename + "/" + filename + '.png')

    plt.figure()
    plt.plot(xdata_O1, f( xdata_O1, *popt_O1), 'g-', label='O1 fit: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt_O1))
    plt.legend()
    plt.savefig("./graficos/" + filename + "/" + filename + '_O1.png')

    plt.figure()
    plt.plot(xdata_O2, f( xdata_O2, *popt_O2), 'y-', label='O2 fit: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt_O2))
    plt.legend()
    plt.savefig("./graficos/" + filename + "/" + filename + '_O2.png')

    plt.figure()
    plt.plot(xdata, f( xdata, *popt), label='fit: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt))
    plt.plot(xdata_O1, f( xdata_O1, *popt_O1), 'g-', label='O1 fit: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt_O1))
    plt.plot(xdata_O2, f( xdata_O2, *popt_O2), 'y-', label='O2 fit: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt_O2))
    plt.legend()
    plt.savefig("./graficos/" + filename + "/" + filename + '_juntas.png')

#====================ALGORITMOS BURBUJA====================

cuadratic_functions("burbuja", 25, 500, cuadratic)
cuadratic_functions("insercion", 25, 500, cuadratic)
cuadratic_functions("seleccion", 25, 500, cuadratic)

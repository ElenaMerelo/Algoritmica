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
    return a*np.log(np.absolute(b)*x)+c

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

def graphics(filename, n_steps, n, f, n_parameters):
    
    compile(filename)

    data = data_generator("./bin/" + filename, 0, n_steps, n)
    w = [ [ data[0][i], data[1][i] ] for i in range(n_steps) ]
    write_csv("./data/"+ filename + "/" + filename + ".csv", w)

    data_O1 = data_generator("./bin/" + filename + "_O1 ", 0, n_steps, n)
    w = [ [ data_O1[0][i], data_O1[1][i] ] for i in range(n_steps) ]
    write_csv("./data/"+ filename + "/" + filename + "_O1.csv", w)

    data_O2 = data_generator("./bin/" + filename + "_O2 ", 0, n_steps, n)
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

    if n_parameters == 3:
        label='a=%5.9f, b=%5.9f, c=%5.9f'
    else:
        label='a=%5.9f, b=%5.9f, c=%5.9f, d=%5.9f'
        
    plt.figure()
    plt.plot(xdata, f( xdata, *popt), label='fit: ' + label % tuple(popt))
    plt.legend()
    plt.savefig("./graficos/" + filename + "/" + filename + '.png')

    plt.figure()
    plt.plot(xdata_O1, f( xdata_O1, *popt_O1), 'g-', label='O1 fit: ' + label % tuple(popt_O1))
    plt.legend()
    plt.savefig("./graficos/" + filename + "/" + filename + '_O1.png')

    plt.figure()
    plt.plot(xdata_O2, f( xdata_O2, *popt_O2), 'y-', label='O2 fit: ' + label % tuple(popt_O2))
    plt.legend()
    plt.savefig("./graficos/" + filename + "/" + filename + '_O2.png')

    plt.figure()
    plt.plot(xdata, f( xdata, *popt), label='fit: ' + label % tuple(popt))
    plt.plot(xdata_O1, f( xdata_O1, *popt_O1), 'g-', label='O1 fit: ' + label % tuple(popt_O1))
    plt.plot(xdata_O2, f( xdata_O2, *popt_O2), 'y-', label='O2 fit: ' + label % tuple(popt_O2))
    plt.legend()
    plt.savefig("./graficos/" + filename + "/" + filename + '_juntas.png')

    plt.close('all')
    os.system("rm ./bin/*")

    latex_table_generator(filename)

def latex_table_generator(filename):
    data = read_csv('./data/'+filename+'/'+filename+'.csv')
    data_O1 = read_csv('./data/'+filename+'/'+filename+'_O1.csv')
    data_O2 = read_csv('./data/'+filename+'/'+filename+'_O2.csv')

    inc1="\n\includegraphics[width=.4\\textwidth]{../graficos/"+filename+"/"+filename+".png}"
    inc2="\n\includegraphics[width=.4\\textwidth]{../graficos/"+filename+"/"+filename+"_O1.png}"
    inc3="\n\includegraphics[width=.4\\textwidth]{../graficos/"+filename+"/"+filename+"_O2.png}"
    inc4="\n\includegraphics[width=.6\\textwidth]{../graficos/"+filename+"/"+filename+"_juntas.png}"
    
    graph = "\\newpage\n\subsection{Algoritmo "+filename+"}"+"\n\\begin{center}"+inc1+inc2+inc3+"\n\end{center}\n\\begin{center}"+inc4+"\n\end{center}\n"

    header = graph + "\\begin{center}\n\\begin{tabular}{| c | c | c | c |}\n\hline\n\\textbf{N} & \\textbf{O-} & \\textbf{O1} & \\textbf{O2} \\\\ \hline\n"

    table = [ str(data[i][0] + " & " + data[i][1] + " & " + data_O1[i][1] + " & " + data_O2[i][1] + " \\\\ \\hline\n") for i in range(25) ]
    for i in range(25):
        header+=table[i]
    header+="\hline\n\end{tabular}\n\end{center}\n"

    text_file = open("./memoria/tables/"+filename+".tex", 'w')
    text_file.write(header)
    text_file.close()

#====================*====================

# graphics("burbuja", 25, 500, cuadratic, 3)
# print 'Burbuja done.'

# graphics("insercion", 25, 500, cuadratic, 3)
# print 'Insercion done.'

# graphics("seleccion", 25, 500, cuadratic, 3)
# print 'Seleccion done.'

# graphics("heapsort", 25, 500, n_log_n, 3)
# print 'Heapsort done.'

# graphics("mergesort", 25, 500, n_log_n, 3)
# print 'Mergesort done.'

# graphics("quicksort", 25, 500, n_log_n, 3)
# print 'Quicksort done.'

# graphics("floyd", 25, 5, cubic, 4)
# print 'Floyd done.'

# graphics("hanoi", 25, 0.5, two_raised_to_the_n, 3)
# print 'Hanoi done.'

#====================*====================

quick_sort_data = read_csv("./data/quicksort/quicksort_O2.csv")
quick_sort = [ [int(x[0]) for x in quick_sort_data ], [float(x[1]) for x in quick_sort_data ] ]

merge_sort_data = read_csv("./data/mergesort/mergesort_O2.csv")
merge_sort = [ [int(x[0]) for x in merge_sort_data ], [float(x[1]) for x in merge_sort_data ] ]

heap_sort_data = read_csv("./data/heapsort/heapsort_O2.csv")
heap_sort = [ [int(x[0]) for x in heap_sort_data ], [float(x[1]) for x in heap_sort_data ] ]

seleccion_data = read_csv("./data/seleccion/seleccion_O2.csv")
seleccion = [ [int(x[0]) for x in seleccion_data ], [float(x[1]) for x in seleccion_data ] ]

insercion_data = read_csv("./data/insercion/insercion_O2.csv")
insercion = [ [int(x[0]) for x in insercion_data ], [float(x[1]) for x in insercion_data ] ]

burbuja_data = read_csv("./data/burbuja/burbuja_O2.csv")
burbuja = [ [int(x[0]) for x in burbuja_data ], [float(x[1]) for x in burbuja_data ] ]


x = array(quick_sort[0])
y = array(quick_sort[1])
x1 = array(merge_sort[0])
y1 = array(merge_sort[1])
x2 = array(heap_sort[0])
y2 = array(heap_sort[1])
x3 = array(seleccion[0])
y3 = array(seleccion[1])
x4 = array(insercion[0])
y4 = array(insercion[1])
x5 = array(burbuja[0])
y5 = array(burbuja[1])


popt, pcov = curve_fit(n_log_n, x, y)
popt1, pcov1 = curve_fit(n_log_n, x1, y1)
popt2, pcov2 = curve_fit(n_log_n, x2, y2)
popt3, pcov3 = curve_fit(cuadratic, x3, y3)
popt4, pcov4 = curve_fit(cuadratic, x4, y4)
popt5, pcov5 = curve_fit(cuadratic, x5, y5)


plt.figure()
plt.plot(x, n_log_n( x, *popt), 'y-', label='Quicksort: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt))
plt.plot(x1, n_log_n( x1, *popt1), 'b-', label='Mergesort: a=%5.9f, b=%5.9f, c=%5.9f ' % tuple(popt1))
plt.plot(x2, n_log_n( x2, *popt2), 'g-', label='Heapsort: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt2))
plt.plot(x3, cuadratic( x3, *popt3), 'r-', label='Seleccion: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt3))
plt.plot(x4, cuadratic( x4, *popt4), 'c-', label='Insercion: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt4))
plt.plot(x5, cuadratic( x5, *popt5), 'm-', label='Burbuja: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt5))

plt.legend()
plt.savefig('./graficos/ordenacion/ordenacion.png')

plt.close('all')

print "Sort Graphic 1 done"

#====================*====================

plt.figure()
plt.plot(x, n_log_n( x, *popt), 'y-', label='Quicksort: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt))
plt.plot(x1, n_log_n( x1, *popt1), 'b-', label='Mergesort: a=%5.9f, b=%5.9f, c=%5.9f ' % tuple(popt1))
plt.plot(x2, n_log_n( x2, *popt2), 'g-', label='Heapsort: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt2))

plt.legend()
plt.savefig('./graficos/ordenacion/ordenacion_sort_only.png')

plt.close('all')

print "Sort Graphic 2 done"

#====================*====================

popt_a, pcov_a = curve_fit(cuadratic, x5, y5)
popt1_a, pcov1_a = curve_fit(cubic, x5, y5)
popt2_a, pcov2_a = curve_fit(n_log_n, x5, y5)

plt.figure()
plt.plot(x5, cuadratic( x5, *popt_a), 'y-', label='x^2: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt_a))
plt.plot(x5, cubic( x5, *popt1_a), 'b-', label='x^3: a=%5.9f, b=%5.9f, c=%5.9f, d=%5.9f ' % tuple(popt1_a))
plt.plot(x5, n_log_n( x5, *popt2_a), 'g-', label='n_log_n: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt2_a))
plt.scatter(x5, y5)

plt.legend()
plt.savefig('./graficos/ordenacion/ajuste.png')

plt.close('all')

print "Adjustment Graphic done"
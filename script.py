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

def square_root(x, a, b):
    return a*np.sqrt(x)+b

#====================COMPILACION====================

def compile(filename):
    os.system("g++ ./source/"+filename+".cpp -o ./bin/"+filename+" -std=gnu++0x")
    os.system("g++ -O1 ./source/"+filename+".cpp -o ./bin/"+filename+"_O1 -std=gnu++0x")
    os.system("g++ -O2 ./source/"+filename+".cpp -o ./bin/"+filename+"_O2 -std=gnu++0x")

#====================GENERACION DE GRAFICOS====================

def graphics(filename, n_steps, n, f, n_parameters):

    compile(filename)

    data = data_generator("./bin/" + filename, 0, n_steps, n)
    w = [ [ data[0][i], data[1][i] ] for i in range(n_steps) ]
    write_csv("./data/"+ filename + "/" + filename + "_antonio.csv", w)

    data_O1 = data_generator("./bin/" + filename + "_O1 ", 0, n_steps, n)
    w = [ [ data_O1[0][i], data_O1[1][i] ] for i in range(n_steps) ]
    write_csv("./data/"+ filename + "/" + filename + "_O1_antonio.csv", w)

    data_O2 = data_generator("./bin/" + filename + "_O2 ", 0, n_steps, n)
    w = [ [ data_O2[0][i], data_O2[1][i] ] for i in range(n_steps) ]
    write_csv("./data/"+ filename + "/" + filename + "_O2_antonio.csv", w)

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
    plt.savefig("./graficos/" + filename + "/" + filename + '_antonio.png')

    plt.figure()
    plt.plot(xdata_O1, f( xdata_O1, *popt_O1), 'g-', label='O1 fit: ' + label % tuple(popt_O1))
    plt.legend()
    plt.savefig("./graficos/" + filename + "/" + filename + '_O1_antonio.png')

    plt.figure()
    plt.plot(xdata_O2, f( xdata_O2, *popt_O2), 'y-', label='O2 fit: ' + label % tuple(popt_O2))
    plt.legend()
    plt.savefig("./graficos/" + filename + "/" + filename + '_O2_antonio.png')

    plt.figure()
    plt.plot(xdata, f( xdata, *popt), label='fit: ' + label % tuple(popt))
    plt.plot(xdata_O1, f( xdata_O1, *popt_O1), 'g-', label='O1 fit: ' + label % tuple(popt_O1))
    plt.plot(xdata_O2, f( xdata_O2, *popt_O2), 'y-', label='O2 fit: ' + label % tuple(popt_O2))
    plt.legend()
    plt.savefig("./graficos/" + filename + "/" + filename + '_juntas_antonio.png')

    plt.close('all')
    os.system("rm ./bin/*")

    latex_table_generator(filename)

def latex_table_generator(filename):
    data = read_csv('./data/'+filename+'/'+filename+'_antonio.csv')
    data_O1 = read_csv('./data/'+filename+'/'+filename+'_O1_antonio.csv')
    data_O2 = read_csv('./data/'+filename+'/'+filename+'_O2_antonio.csv')

    inc1="\n\includegraphics[width=.4\\textwidth]{../graficos/"+filename+"/"+filename+"_antonio.png}"
    inc2="\n\includegraphics[width=.4\\textwidth]{../graficos/"+filename+"/"+filename+"_O1_antonio.png}"
    inc3="\n\includegraphics[width=.4\\textwidth]{../graficos/"+filename+"/"+filename+"_O2_antonio.png}"
    inc4="\n\includegraphics[width=.6\\textwidth]{../graficos/"+filename+"/"+filename+"_juntas_antonio.png}"

    graph = "\\newpage\n\subsection{Algoritmo "+filename+"}"+"\n\\begin{center}"+inc1+inc2+inc3+"\n\end{center}\n\\begin{center}"+inc4+"\n\end{center}\n"

    header = graph + "\\begin{center}\n\\begin{tabular}{| c | c | c | c |}\n\hline\n\\textbf{N} & \\textbf{O-} & \\textbf{O1} & \\textbf{O2} \\\\ \hline\n"

    table = [ str(data[i][0] + " & " + data[i][1] + " & " + data_O1[i][1] + " & " + data_O2[i][1] + " \\\\ \\hline\n") for i in range(len(data)) ]
    for i in range(len(data)):
        header+=table[i]
    header+="\hline\n\end{tabular}\n\end{center}\n"

    text_file = open("./memoria/tables/"+filename+"_antonio.tex", 'w')
    text_file.write(header)
    text_file.close()

#====================*====================

# graphics("burbuja", 35, 1000, cuadratic, 3)
# print 'Burbuja done.'

# graphics("insercion", 35, 1000, cuadratic, 3)
# print 'Insercion done.'

# graphics("seleccion", 35, 1000, cuadratic, 3)
# print 'Seleccion done.'

# graphics("heapsort", 35, 1000, n_log_n, 3)
# print 'Heapsort done.'

# graphics("mergesort", 35, 1000, n_log_n, 3)
# print 'Mergesort done.'

# graphics("quicksort", 35, 1000, n_log_n, 3)
# print 'Quicksort done.'

# graphics("floyd", 35, 10, cubic, 4)
# print 'Floyd done.'

# graphics("hanoi", 25, 1, two_raised_to_the_n, 3)
# print 'Hanoi done.'

# #====================*====================

quick_sort_data = read_csv("./data/quicksort/quicksort_O2_antonio.csv")
quick_sort = [ [int(x[0]) for x in quick_sort_data ], [float(x[1]) for x in quick_sort_data ] ]

merge_sort_data = read_csv("./data/mergesort/mergesort_O2_antonio.csv")
merge_sort = [ [int(x[0]) for x in merge_sort_data ], [float(x[1]) for x in merge_sort_data ] ]

heap_sort_data = read_csv("./data/heapsort/heapsort_O2_antonio.csv")
heap_sort = [ [int(x[0]) for x in heap_sort_data ], [float(x[1]) for x in heap_sort_data ] ]

seleccion_data = read_csv("./data/seleccion/seleccion_O2_antonio.csv")
seleccion = [ [int(x[0]) for x in seleccion_data ], [float(x[1]) for x in seleccion_data ] ]

insercion_data = read_csv("./data/insercion/insercion_O2_antonio.csv")
insercion = [ [int(x[0]) for x in insercion_data ], [float(x[1]) for x in insercion_data ] ]

burbuja_data = read_csv("./data/burbuja/burbuja_O2_antonio.csv")
burbuja = [ [int(x[0]) for x in burbuja_data ], [float(x[1]) for x in burbuja_data ] ]

floyd_data = read_csv("./data/floyd/floyd_O2_antonio.csv")
floyd = [ [int(x[0]) for x in floyd_data ], [float(x[1]) for x in floyd_data ] ]

hanoi_data = read_csv("./data/hanoi/hanoi_O2_antonio.csv")
hanoi = [ [int(x[0]) for x in hanoi_data ], [float(x[1]) for x in hanoi_data ] ]

quick_sort_data = read_csv("./data/quicksort/quicksort_O2_elena.csv")
quick_sort_e = [ [int(x[0]) for x in quick_sort_data ], [float(x[1]) for x in quick_sort_data ] ]

merge_sort_data = read_csv("./data/mergesort/mergesort_O2_elena.csv")
merge_sort_e = [ [int(x[0]) for x in merge_sort_data ], [float(x[1]) for x in merge_sort_data ] ]

heap_sort_data = read_csv("./data/heapsort/heapsort_O2_elena.csv")
heap_sort_e = [ [int(x[0]) for x in heap_sort_data ], [float(x[1]) for x in heap_sort_data ] ]

seleccion_data = read_csv("./data/seleccion/seleccion_O2_elena.csv")
seleccion_e = [ [int(x[0]) for x in seleccion_data ], [float(x[1]) for x in seleccion_data ] ]

insercion_data = read_csv("./data/insercion/insercion_O2_elena.csv")
insercion_e = [ [int(x[0]) for x in insercion_data ], [float(x[1]) for x in insercion_data ] ]

burbuja_data = read_csv("./data/burbuja/burbuja_O2_elena.csv")
burbuja_e = [ [int(x[0]) for x in burbuja_data ], [float(x[1]) for x in burbuja_data ] ]

floyd_data = read_csv("./data/floyd/floyd_O2_elena.csv")
floyd_e = [ [int(x[0]) for x in floyd_data ], [float(x[1]) for x in floyd_data ] ]

hanoi_data = read_csv("./data/hanoi/hanoi_O2_elena.csv")
hanoi_e = [ [int(x[0]) for x in hanoi_data ], [float(x[1]) for x in hanoi_data ] ]



x_a = array(quick_sort[0])
y_a = array(quick_sort[1])
x1_a = array(merge_sort[0])
y1_a = array(merge_sort[1])
x2_a = array(heap_sort[0])
y2_a = array(heap_sort[1])
x3_a = array(seleccion[0])
y3_a = array(seleccion[1])
x4_a = array(insercion[0])
y4_a = array(insercion[1])
x5_a = array(burbuja[0])
y5_a = array(burbuja[1])
x6_a = array(floyd[0])
y6_a = array(floyd[1])
x7_a = array(hanoi[0])
y7_a = array(hanoi[1])


x_e = array(quick_sort_e[0])
y_e = array(quick_sort_e[1])
x1_e = array(merge_sort_e[0])
y1_e = array(merge_sort_e[1])
x2_e = array(heap_sort_e[0])
y2_e = array(heap_sort_e[1])
x3_e = array(seleccion_e[0])
y3_e = array(seleccion_e[1])
x4_e = array(insercion_e[0])
y4_e = array(insercion_e[1])
x5_e = array(burbuja_e[0])
y5_e = array(burbuja_e[1])
x6_e = array(floyd_e[0])
y6_e = array(floyd_e[1])
x7_e = array(hanoi_e[0])
y7_e = array(hanoi_e[1])



popt_a, pcov = curve_fit(n_log_n, x_a, y_a)
popt1_a, pcov1 = curve_fit(n_log_n, x1_a, y1_a)
popt2_a, pcov2 = curve_fit(n_log_n, x2_a, y2_a)
popt3_a, pcov3 = curve_fit(cuadratic, x3_a, y3_a)
popt4_a, pcov4 = curve_fit(cuadratic, x4_a, y4_a)
popt5_a, pcov5 = curve_fit(cuadratic, x5_a, y5_a)
popt6_a, pcov6 = curve_fit(cubic, x6_a, y6_a)
popt7_a, pcov7 = curve_fit(two_raised_to_the_n, x7_a, y7_a)

popt_e, pcov = curve_fit(n_log_n, x_e, y_e)
popt1_e, pcov1 = curve_fit(n_log_n, x1_e, y1_e)
popt2_e, pcov2 = curve_fit(n_log_n, x2_e, y2_e)
popt3_e, pcov3 = curve_fit(cuadratic, x3_e, y3_e)
popt4_e, pcov4 = curve_fit(cuadratic, x4_e, y4_e)
popt5_e, pcov5 = curve_fit(cuadratic, x5_e, y5_e)
popt6_e, pcov6 = curve_fit(cubic, x6_e, y6_e)
popt7_e, pcov7 = curve_fit(two_raised_to_the_n, x7_e, y7_e)


plt.figure()
plt.plot(x_a, n_log_n( x_a, *popt_a), 'b-', label='Quicksort: Antonio')
plt.plot(x_e, n_log_n( x_e, *popt_e), 'r-', label='Quicksort: Elena')
plt.legend()
plt.savefig('./graficos/distintos/quicksort.png')
plt.close('all')

plt.figure()
plt.plot(x1_a, n_log_n( x1_a, *popt1_a), 'b-', label='Mergesort: Antonio')
plt.plot(x1_e, n_log_n( x1_e, *popt1_e), 'r-', label='Mergesort: Elena')
plt.legend()
plt.savefig('./graficos/distintos/mergesort.png')
plt.close('all')

plt.figure()
plt.plot(x2_a, n_log_n( x2_a, *popt2_a), 'b-', label='Heapsort: Antonio')
plt.plot(x2_e, n_log_n( x2_e, *popt2_e), 'r-', label='Heapsort: Elena')
plt.legend()
plt.savefig('./graficos/distintos/heapsort.png')
plt.close('all')

plt.figure()
plt.plot(x3_a, cuadratic( x3_a, *popt3_a), 'b-', label='Seleccion: Antonio')
plt.plot(x3_e, cuadratic( x3_e, *popt3_e), 'r-', label='Seleccion: Elena')
plt.legend()
plt.savefig('./graficos/distintos/seleccion.png')
plt.close('all')

plt.figure()
plt.plot(x4_a, cuadratic( x4_a, *popt4_a), 'b-', label='Insercion: Antonio')
plt.plot(x4_e, cuadratic( x4_e, *popt4_e), 'r-', label='Insercion: Elena')
plt.legend()
plt.savefig('./graficos/distintos/insercion.png')
plt.close('all')

plt.figure()
plt.plot(x5_a, cuadratic( x5_a, *popt5_a), 'b-', label='Burbuja: Antonio')
plt.plot(x5_e, cuadratic( x5_e, *popt5_e), 'r-', label='Burbuja: Elena')
plt.legend()
plt.savefig('./graficos/distintos/burbuja.png')
plt.close('all')

plt.figure()
plt.plot(x6_a, cubic( x6_a, *popt6_a), 'b-', label='Floyd: Antonio')
plt.plot(x6_e, cubic( x6_e, *popt6_e), 'r-', label='Floyd: Elena')
plt.legend()
plt.savefig('./graficos/distintos/floyd.png')
plt.close('all')

plt.figure()
plt.plot(x7_a, two_raised_to_the_n( x7_a, *popt7_a), 'b-', label='Hanoi: Antonio')
plt.plot(x7_e, two_raised_to_the_n( x7_e, *popt7_e), 'r-', label='Hanoi: Elena')
plt.legend()
plt.savefig('./graficos/distintos/Hanoi.png')
plt.close('all')


# plt.figure()
# plt.plot(x, n_log_n( x, *popt), 'y-', label='Quicksort: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt))
# plt.plot(x1, n_log_n( x1, *popt1), 'b-', label='Mergesort: a=%5.9f, b=%5.9f, c=%5.9f ' % tuple(popt1))
# plt.plot(x2, n_log_n( x2, *popt2), 'g-', label='Heapsort: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt2))
# plt.plot(x3, cuadratic( x3, *popt3), 'r-', label='Seleccion: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt3))
# plt.plot(x4, cuadratic( x4, *popt4), 'c-', label='Insercion: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt4))
# plt.plot(x5, cuadratic( x5, *popt5), 'm-', label='Burbuja: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt5))

# plt.legend()
# plt.savefig('./graficos/ordenacion/ordenacion_antonio.png')

# plt.close('all')

# print "Sort Graphic 1 done."

#====================*====================

# plt.figure()
# plt.plot(x, n_log_n( x, *popt), 'y-', label='Quicksort: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt))
# plt.plot(x1, n_log_n( x1, *popt1), 'b-', label='Mergesort: a=%5.9f, b=%5.9f, c=%5.9f ' % tuple(popt1))
# plt.plot(x2, n_log_n( x2, *popt2), 'g-', label='Heapsort: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt2))

# plt.legend()
# plt.savefig('./graficos/ordenacion/ordenacion_sort_only_antonio.png')

# plt.close('all')

# print "Sort Graphic 2 done."

#====================*====================

# popt_a, pcov_a = curve_fit(square_root, x5, y5)
# popt1_a, pcov1_a = curve_fit(cubic, x5, y5)
# popt2_a, pcov2_a = curve_fit(n_log_n, x5, y5)

# plt.figure()
# plt.plot(x5, cubic( x5, *popt1_a), 'b-', label='x^3: a=%5.9f, b=%5.9f, c=%5.9f, d=%5.9f ' % tuple(popt1_a))
# plt.plot(x5, n_log_n( x5, *popt2_a), 'g-', label='n_log_n: a=%5.9f, b=%5.9f, c=%5.9f' % tuple(popt2_a))
# plt.plot(x5, square_root( x5, *popt_a), 'y-', label='a*Sqrt(x)+b: a=%5.9f, b=%5.9f' % tuple(popt_a))
# plt.scatter(x5, y5)

# plt.legend()
# plt.savefig('./graficos/ordenacion/ajuste_antonio.png')

# plt.close('all')

# print "Adjustment Graphic done."


x
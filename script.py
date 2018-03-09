#!/usr/bin/env python

import os
import subprocess
import csv

#====================FUNCIONES AUXILIARES====================

def write_csv(filename, data):
    file = open("./data/"+str(filename), 'w')
    with file:
        writer = csv.writer(file)
        writer.writerows(data)

def data_generator(command_line, begin, n_steps, step):
    data=[]
    n=begin

    for i in range(n_steps):
        n+=step
        output = subprocess.check_output(command_line+str(n), shell=True)
        data.append(output.split())

    return data


#====================ALGORITMO BURBUJA====================

os.system("g++ ./source/burbuja.cpp -o ./bin/burbuja -std=gnu++0x")
data_burbuja = data_generator("/home/antonio/Algoritmica/bin/burbuja ", 0, 25, 1)
write_csv("data_burbuja.csv", data_burbuja)

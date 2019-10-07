#!/usr/bin/env python
#* This file is part of the RACCOON application
#* being developed at Dolbow lab at Duke University
#* http://dolbow.pratt.duke.edu

import os
import math
import csv
import sys
import matplotlib.pyplot as plt

def column(matrix, i):
    return [row[i] for row in matrix]

E = 4
nu = 0.2
Gc = 8e-4
mu = E/2/(1+nu)
m = 5
b = 0.1
h = math.sqrt(mu/m/b)

# f, (ax1, ax2) = plt.subplots(1, 2)

prefix = sys.argv[1]
postfix = sys.argv[3]
num_samples = int(sys.argv[2])

xx = []
yy = []

for n in range(num_samples):
    with open(prefix+str(n+1)+postfix) as csv_file:
        x = []
        y = []
        csv_reader = csv.reader(csv_file, delimiter=' ')
        # skip header
        csv_reader.next()

        for row in csv_reader:
            sigma = float(row[0])/1000
            A = float(row[2])
            # A_error = float(row[3])
            D = math.sqrt((1-nu*nu)*h/E/Gc)*sigma
            l = math.sqrt(A)/h
            # l_error = A_error/2/A
            x.append(D)
            y.append(l)
            # y_error.append(l_error)

    # plt.errorbar(x,y,y_error)
    # plt.semilogy(x,y)
    xx.append(x)
    yy.append(y)

min_y = []
max_y = []
mean_y = []

for i in range(len(xx[0])):
    min_y.append(min(column(yy,i)))
    max_y.append(max(column(yy,i)))
    mean_y.append(sum(column(yy,i))/num_samples)

plt.semilogy(xx[0],mean_y)
plt.semilogy(xx[0],min_y)
plt.semilogy(xx[0],max_y)
plt.show()
# plt.savefig("meta_plot.png",dpi=300)

with open(sys.argv[4], mode='w') as numerical_file:
    writer = csv.writer(numerical_file, delimiter=' ', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    for i in range(len(xx[0])):
        writer.writerow([xx[0][i],mean_y[i],min_y[i],max_y[i]])

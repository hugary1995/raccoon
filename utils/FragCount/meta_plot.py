#!/usr/bin/env python
#* This file is part of the RACCOON application
#* being developed at Dolbow lab at Duke University
#* http://dolbow.pratt.duke.edu

import os
import math
import csv
import sys
import random
import matplotlib.pyplot as plt

E = 4
nu = 0.2
Gc = 8e-4
mu = E/2/(1+nu)
m = 5
b = 0.1
h = math.sqrt(mu/m/b)

# f, (ax1, ax2) = plt.subplots(1, 2)

with open(sys.argv[1]) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=' ')
    # skip header
    csv_reader.next()
    # read first line of data
    row = csv_reader.next()
    sigma = float(row[0])/1000
    A = float(row[2])
    # A_error = float(row[3])
    D = math.sqrt((1-nu*nu)*h/E/Gc)*sigma
    l = math.log(math.sqrt(A)/h)
    # l_error = A_error/2/A
    x = [D]
    y = [l]
    # y_error = [l_error]

    for row in csv_reader:
        sigma = float(row[0])/1000
        A = float(row[2])
        # A_error = float(row[3])
        D = math.sqrt((1-nu*nu)*h/E/Gc)*sigma
        l = math.log(math.sqrt(A)/h)
        # l_error = A_error/2/A
        x.append(D)
        y.append(l)
        # y_error.append(l_error)

# plt.errorbar(x,y,y_error)
plt.semilogy(x,y)
plt.show()
# plt.savefig("meta_plot.png",dpi=300)

# with open(sys.argv[2]+'.csv', mode='w') as numerical_file:
#     writer = csv.writer(numerical_file, delimiter=' ', quotechar='"', quoting=csv.QUOTE_MINIMAL)
#     for i in range(len(x)):
#         writer.writerow([x[i],y[i],])

#!/usr/bin/env python
# * This file is part of the RACCOON application
# * being developed at Dolbow lab at Duke University
# * http://dolbow.pratt.duke.edu

import os
import math
import csv
import sys
import matplotlib.pyplot as plt


def column(matrix, i):
    return [row[i] for row in matrix]


def row(matrix, i):
    return matrix[i]


cs = ['4', '6', '8']
Gc = 2.7


for c in cs:
    a = []
    JR = []
    with open(c + '.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        # skip header
        csv_reader.next()
        # skip first step
        csv_reader.next()

        for row in csv_reader:
            a.append(float(row[0]))
            JR.append(float(row[1]) * 2 / Gc)

        plt.plot(a, JR)


plt.ylim(0, 2)
plt.legend(cs)
plt.show()

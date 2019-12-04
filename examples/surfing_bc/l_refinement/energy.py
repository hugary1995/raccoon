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


a = []
elastic_energy = []
fracture_energy = []
plastic_work = []
external_work = []
initial_fracture_energy = 0.54


with open(sys.argv[1]) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    # skip header
    csv_reader.next()
    # skip first step
    csv_reader.next()

    for row in csv_reader:
        a.append(float(row[0]))
        elastic_energy.append(float(row[2]) * 2)
        fracture_energy.append(
            float(row[2]) * 2 + float(row[4]) * 2 - initial_fracture_energy)
        plastic_work.append(
            float(row[2]) * 2 + float(row[4]) * 2 - initial_fracture_energy + float(row[5]) * 2)
        external_work.append(float(row[3]) * 2)


plt.fill_between(a, elastic_energy)
plt.fill_between(a, elastic_energy, fracture_energy)
plt.fill_between(a, fracture_energy, plastic_work)
plt.plot(a, external_work, color='black')
plt.legend(['external work', 'elastic energy', 'fracture dissipation',
            'plastic dissipation'])
plt.show()

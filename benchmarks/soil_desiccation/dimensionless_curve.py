#* This file is part of the RACCOON application
#* being developed at Dolbow lab at Duke University
#* http://dolbow.pratt.duke.edu

import os
import math
import csv
import sys
import matplotlib.pyplot as plt

E = 4
nu = 0.2
Gc = 8e-4
mu = E / 2 / (1 + nu)
m = 5
b = 0.1
h = math.sqrt(mu / m / b)

D_stars = []
l_star_bars = []
for n in range(220):
    with open("count/out_feature_volumes_" + "{:04d}".format(n + 1) + ".csv") as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        next(csv_reader)

        volumes = []
        for row in csv_reader:
            volumes.append(float(row[0]))

        total_volume = sum(volumes)
        scale = 10000 / total_volume

        sigma = (n + 1) / 1000
        D_star = math.sqrt((1 - nu * nu) * h / E / Gc) * sigma
        D_stars.append(D_star)

        l_star_bar = 0
        for volume in volumes:
            l_star_bar += math.sqrt(scale * volume) / h
        l_star_bar /= len(volumes)
        l_star_bars.append(l_star_bar)

plt.semilogy(D_stars, l_star_bars)
plt.show()

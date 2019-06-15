#!/usr/bin/env python
#* This file is part of the RACCOON application
#* being developed at Dolbow lab at Duke University
#* http://dolbow.pratt.duke.edu

import os
import math
import csv
import matplotlib.pyplot as plt

# simulation data

with open("meta.csv") as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    # skip header
    csv_reader.next()
    # read first line of data
    row = csv_reader.next()
    L = [float(row[0])]
    sigma = [float(row[1])]
    for row in csv_reader:
        L.append(float(row[0]))
        sigma.append(float(row[1]))

# analytical solution

E_film = 4.0
nu_film = 0.2
E_substrate = 800.0
nu_substrate = 0.2
Gc = 8e-4
h = 1.0
E1 = E_film/(1.0-nu_film*nu_film)
E0 = E_substrate/(1.0-nu_substrate*nu_substrate)
d = math.pi/2.0
c = d*math.sqrt((1.0-nu_film)/2.0)
L_analytical = []
sigma_analytical = []
for i in range(1000):
    L_curr = pow(10.0,i/1000.0)
    L_analytical.append(L_curr)
    sigma_curr = math.sqrt(Gc*E1*c/h/(2.0*math.tanh(c*L_curr/2.0/h)-math.tanh(c*L_curr/h)))
    sigma_analytical.append(sigma_curr)

plt.semilogx(L,sigma,'k+',L_analytical,sigma_analytical,'k-')
plt.show()

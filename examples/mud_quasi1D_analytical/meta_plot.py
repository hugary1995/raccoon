#!/usr/bin/env python
#* This file is part of the RACCOON application
#* being developed at Dolbow lab at Duke University
#* http://dolbow.pratt.duke.edu

import os
import math
import csv
import matplotlib.pyplot as plt

# Youngs modulus ratios
Eratio = ["0.5","1","2","5","10"];
Eratio_legend = [];

# two subplots
f, (ax1, ax2) = plt.subplots(1, 2, sharey=True)

for R in Eratio:

    Eratio_legend.append("$E_1/E_0$ = "+R)

    # analytical solution

    E_film = 4.0
    nu_film = 0.2
    E_substrate = E_film*float(R)
    nu_substrate = 0.2
    Gc = 8e-4
    h = 1.0
    E1 = E_film/(1.0-nu_film*nu_film)
    E0 = E_substrate/(1.0-nu_substrate*nu_substrate)
    mu1 = E_film/2.0/(1-nu_film)
    mu0 = E_substrate/2.0/(1-nu_substrate)
    alpha = (E1-E0)/(E1+E0)
    beta = (mu1*(1.0-2.0*mu0)-mu0*(1.0-2.0*mu1))/(2.0*mu1*(1.0-mu0)+2.0*mu0*(1.0-mu1))
    g = (1.258-0.4*alpha-0.26*pow(alpha,3.0)-0.3*pow(alpha,4.0))/(1.0-alpha)
    c = 2.0/math.pi/g
    d = math.sqrt(E1/mu1)*c

    # simulation data

    with open("script_varying_L_Eratio_"+R+"/meta.csv") as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        # skip header
        csv_reader.next()
        # read first line of data
        row = csv_reader.next()
        L = [float(row[0])]
        sigma = float(row[1])
        driving = [sigma*math.sqrt(h/E1/Gc)]
        for row in csv_reader:
            L.append(float(row[0]))
            sigma = float(row[1])
            driving.append(sigma*math.sqrt(h/E1/Gc))


    L_analytical = []
    driving_analytical = []
    for i in range(140):
        L_curr = pow(10.0,0.3+i/100.0)
        L_analytical.append(L_curr)
        sigma_curr = math.sqrt(Gc*E1*c/h/(2.0*math.tanh(c*L_curr/2.0/h)-math.tanh(c*L_curr/h)))
        driving_analytical.append(sigma_curr*math.sqrt(h/E1/Gc))

    ax1.semilogx(L,driving,'o-')
    ax2.semilogx(L_analytical,driving_analytical)

plt.ylim(0,2)

ax1.set_xlabel("Dimensionless Fragment Size $\lambda/h$")
ax1.set_ylabel("Dimensionless Driving Force $\sigma\sqrt{h/E_1G_c}$")
ax1.set_title("FEM")
ax1.legend(Eratio_legend)

ax2.set_xlabel("Dimensionless Fragment Size $\lambda/h$")
ax2.set_ylabel("Dimensionless Driving Force $\sigma\sqrt{h/E_1G_c}$")
ax2.set_title("Analytical")
ax2.legend(Eratio_legend)

plt.show()
# plt.savefig("meta_plot.png",dpi=300)

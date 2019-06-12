#!/usr/bin/env python
#* This file is part of the RACCOON application
#* being developed at Dolbow lab at Duke University
#* http://dolbow.pratt.duke.edu

import sys
import math
import csv

if len(sys.argv) != 1:
    print("usage: ./analytical.py")
    exit()

##################
### parameters ###
##################

# material properties
E_film = 4.0
nu_film = 0.2
Gc = 8e-4

# geometry
h = 1.0
L = 20

###################
### calculation ###
###################

# plane strain
E1 = E_film/(1.0-nu_film*nu_film)

# Dimensionless constants
d = math.pi/2.0
c = d*math.sqrt((1.0-nu_film)/2.0)

# critical stress
critical_stress = math.sqrt(Gc*E1*c/h)
critical_energy = critical_stress*critical_stress/2.0/E1
print("Suggested critical energy = "+str(critical_energy))

# cracks
for i in range(10):
  fragment_size = h*L*pow(0.5,i)
  sigma = math.sqrt(Gc*E1*c/h/(2.0*math.tanh(c*fragment_size/2.0/h)-math.tanh(c*fragment_size/h)))
  print("Crack #"+str(i+1)+", at stress = "+str(sigma))

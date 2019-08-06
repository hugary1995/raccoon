#!/usr/bin/env python
#* This file is part of the RACCOON application
#* being developed at Dolbow lab at Duke University
#* http://dolbow.pratt.duke.edu

import csv
import sys
import matplotlib.pyplot as plt

if len(sys.argv) == 1:
    print("Usage: python ./plot.py file_name.csv")
    sys.exit()

with open(sys.argv[1]) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    # skip header
    csv_reader.next()
    # read first line of data
    row = csv_reader.next()
    t = [float(row[0])]
    u1 = [float(row[1])]
    u2 = [float(row[2])]
    u3 = [float(row[3])]
    u4 = [float(row[4])]
    for row in csv_reader:
        t.append(float(row[0]))
        u1.append(float(row[1]))
        u2.append(float(row[2]))
        u3.append(float(row[3]))
        u4.append(float(row[4]))

plt.plot(t,u1)
plt.plot(t,u2)
plt.plot(t,u3)
plt.plot(t,u4)
plt.legend(["u1","u2","u3","u4"])
plt.show()

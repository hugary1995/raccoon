#!/usr/bin/env python
#* This file is part of the RACCOON application
#* being developed at Dolbow lab at Duke University
#* http://dolbow.pratt.duke.edu

import csv
import sys
import matplotlib.pyplot as plt

if len(sys.argv) == 1:
    print("Usage: python ./plot.py file_name1.csv file_name2.csv file_name3.csv ...")
    sys.exit()

for file_i in range(len(sys.argv)-1):
    with open(sys.argv[file_i+1]) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=' ')
        # skip header
        csv_reader.next()
        # read first line of data
        row = csv_reader.next()
        f = [float(row[1])]
        u = [float(row[2])]
        fpi = [1]
        for row in csv_reader:
            f_ = float(row[1])
            u_ = float(row[2])
            f.append(f_)
            u.append(u_)

    plt.plot(u,f,'.-')

plt.show()

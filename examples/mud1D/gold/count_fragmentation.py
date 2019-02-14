#!/usr/bin/env python

import sys
import csv

if len(sys.argv) == 2:
    threshold = 0.5
elif len(sys.argv) == 3:
    threshold = float(sys.argv[2])
else:
    print("usage: ./count_fragmentation.py file_name [threshold]")
    exit()

with open(sys.argv[1]) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    # skip header
    csv_reader.next()
    # read first line of data
    row = csv_reader.next()
    d = [float(row[4])]
    # initialize fragment count
    count = 0
    for row in csv_reader:
        # previous d
        d_previous = d[len(d)-1]
        # current d
        d_current = float(row[4])
        d.append(d_current)
        # if it passes the threshold and it is increasing
        if (d_previous<threshold) and (d_current>threshold):
            count += 1
    print("number of fragments = "+str(count))

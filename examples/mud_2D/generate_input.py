#!/usr/bin/env python
#* This file is part of the RACCOON application
#* being developed at Dolbow lab at Duke University
#* http://dolbow.pratt.duke.edu

import os
import sys
import shutil
import math
import csv

if len(sys.argv) != 1:
    print("usage: ./generate_input.py")
    exit()

##################
###   listing  ###
##################

run_names = []
with open("gold/list.txt") as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=' ')
    for row in csv_reader:
        run_names.append(row[0])

print(run_names)

with open('gold/master.i', 'r') as input:
    master_input = input.read()
with open('gold/fracture.i', 'r') as input:
    fracture_input = input.read()
with open('gold/mechanical.i', 'r') as input:
    mechanical_input = input.read()
with open('gold/batch.sh', 'r') as input:
    batch_file = input.read()

count = 0

for run_name in run_names:
    count = count+1
    print("---------------------------------------------")
    print("Remove existing run folder: "+run_name)
    try:
        shutil.rmtree(run_name)
    except OSError as e:
        print ("     %s: %s." % (e.filename, e.strerror))

    print("Create new run folder: "+run_name)

    # Create target Directory if don't exist
    if not os.path.exists(run_name):
        os.mkdir(run_name)
    else:
        print("     Directory "+run_name+" already exists")

    Gc_file = "../gold/Gc_"+run_name+".txt"
    psic_file = "../gold/psic_"+run_name+".txt"

    ##################
    ###   master   ###
    ##################

    print("Writing master input file...")
    f = open(run_name+"/master.i", "w")
    f.write(master_input)
    f.close()

    ##################
    ###  fracture  ###
    ##################

    print("Writing fracture input file...")
    f = open(run_name+"/fracture.i", "w")
    f.write(fracture_input % (Gc_file,psic_file))
    f.close()

    ##################
    ### mechanical ###
    ##################

    print("Writing mechanical input file...")
    f = open(run_name+"/mechanical.i", "w")
    f.write(mechanical_input % (Gc_file,psic_file))
    f.close()

    ##################
    ###   batch    ###
    ##################

    print("Writing batch file...")
    f = open(run_name+"/"+str(count), "w")
    f.write(batch_file)
    f.close()

#!/bin/bash

for d in *sample*/*.sh ; do
    sbatch $d
done

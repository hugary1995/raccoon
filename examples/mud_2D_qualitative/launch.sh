#!/bin/bash

for d in *seed*/*.sh ; do
    sbatch $d
done

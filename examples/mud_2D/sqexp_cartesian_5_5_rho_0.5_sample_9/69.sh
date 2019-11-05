#!/bin/bash
#
#SBATCH -o output.txt
#SBATCH -e error.txt
#SBATCH -n 6
#SBATCH -N 1
#SBATCH --mem-per-cpu=10000
#SBATCH -t 3-00:00
#SBATCH -p dolbowlab --account=dolbowlab

cd sqexp_cartesian_5_5_rho_0.5_sample_9
mpiexec /dscrhome/th224/projects/raccoon/raccoon-opt -i master.i

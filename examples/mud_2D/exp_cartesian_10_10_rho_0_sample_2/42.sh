#!/bin/bash
#
#SBATCH -o output.txt
#SBATCH -e error.txt
#SBATCH -n 6
#SBATCH -N 1
#SBATCH --mem-per-cpu=10000
#SBATCH -t 3-00:00
#SBATCH -p dolbowlab --account=dolbowlab

cd exp_cartesian_10_10_rho_0_sample_2
mpiexec /dscrhome/th224/projects/raccoon/raccoon-opt -i master.i

#!/bin/bash
#SBATCH -N 1
#SBATCH --ntasks=16
#SBATCH --job-name=surf_AT2_Gc2L0.70del1.16_dt1e-2_J5b
#SBATCH --partition=dolbowlab
#SBATCH --mem-per-cpu=10G
#SBATCH --mail-user=bo.zeng@duke.edu
#SBATCH --mail-type=END
#SBATCH -o surf_AT2_Gc2L0.70del1.16_dt1e-2_J5b_%j

module load cmake/3.19.3-rhel8
module load Python/3.8.1
module load GCC/9.3.0
module load MPICH/3.2.1
export SLURM_CPU_BIND=none
echo "Start: $(date)"
echo "cwd: $(pwd)"
mpiexec /hpc/home/bz75/projects/raccoon/raccoon-opt -i elasticity.i
echo "End: $(date)"

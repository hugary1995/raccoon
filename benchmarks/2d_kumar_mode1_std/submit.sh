#!/bin/bash
#SBATCH -N 1
#SBATCH --ntasks=8
#SBATCH --job-name=2d_kumar_mode1_std
#SBATCH --partition=dolbowlab
#SBATCH --mem-per-cpu=10G
#SBATCH --mail-user=bo.zeng@duke.edu
#SBATCH --mail-type=END
#SBATCH -o 2d_kumar_mode1_std_%j

module load cmake/3.19.3-rhel8
module load Python/3.8.1
module load GCC/9.3.0
module load MPICH/3.2.1
export SLURM_CPU_BIND=none
echo "Start: $(date)"
echo "cwd: $(pwd)"
mpiexec /hpc/home/bz75/projects/raccoon/raccoon-opt -i elasticity.i
echo "End: $(date)"



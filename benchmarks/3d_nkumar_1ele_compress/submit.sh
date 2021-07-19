#!/bin/bash
#SBATCH -N 1
#SBATCH --ntasks=4
#SBATCH --job-name=check_transfer_nkumar_1ele_c
#SBATCH --partition=dolbowlab
#SBATCH --mem-per-cpu=10G
#SBATCH --mail-user=bo.zeng@duke.edu
#SBATCH --mail-type=END
#SBATCH -o check_transfer_nkumar_1ele_c_%j

module load cmake/3.19.3-rhel8
module load Python/3.8.1
module load GCC/9.3.0
module load MPICH/3.2.1
export SLURM_CPU_BIND=none
echo "Start: $(date)"
echo "cwd: $(pwd)"
mpiexec /hpc/home/bz75/projects/raccoon/raccoon-opt -i elasticity.i
echo "End: $(date)"


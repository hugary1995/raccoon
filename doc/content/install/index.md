# Install

RACCOON is a perfect example of integration of multiple open source projects.

- RACCOON is built upon [MOOSE](https://mooseframework.inl.gov/), an object oriented parallel FEM framework.
- MOOSE is built upon [libMesh](http://libmesh.github.io/), a C++ FEM library, to utilize its FEM basics.
- Both MOOSE and libMesh rely on many other tools, such as [PETSc](https://www.mcs.anl.gov/petsc/).

Therefore, it can be complicated to get RACCOON to work. Fortunately, the installation process
has been thought through and thoroughly tested. The installation process can be summarized in the following steps,
each of which should only take a handful of commands.

!alert note title= Install RACCOON on DCC
Instead of the instructions below, refer to [these instructions](https://github.com/casperversteeg/fuzzy-pancake-eureka/blob/master/DCCmoose.pdf) for setting up RACCOON on the Duke Computing Cluster. Contact [Casper Versteeg](mailto:casper.versteeg@duke.edu) if you need access to these instructions. Contact Professor [John Dolbow](mailto:jdolbow@duke.edu) if you are our group member and need access to DCC.

## 1. Install environment packages

First, follow [these instructions](install/conda.md) to install environment packages required for MOOSE and RACCOON.

## 2. Clone RACCOON

Next, clone RACCOON to your local projects directory:

```bash
mkdir ~/projects
cd ~/projects
git clone https://github.com/hugary1995/raccoon.git
cd raccoon
git checkout master
git submodule update --init
```

These commands should download a copy of RACCOON and a copy of MOOSE (as a submodule) to your local projects directory.

## 3. Compile libMesh (Optional)

This step is only necessary if you didn't choose to include moose-libmesh in your Conda environment.
For example, if you want to compile libMesh with boost, simply do

```bash
cd ~/projects/raccoon
./moose/scripts/update_and_rebuild_libmesh.sh --with-boost
```

## 4. Compile RACCOON

```bash
make -j N
```

where `N` is the number of processors you want to use to compile RACCOON in parallel.

## 5. Test RACCOON

To make sure RACCOON is working properly, run the script `run_tests` to automatically run all regression tests:

```bash
./run_tests -j N
```

Optionally, you may try [benchmark problems](benchmarks/index.md).

## 6. Update RACCOON (Optional)

RACCOON is under rapid development right now. To make sure you get the most out of RACCOON, update your local repository frequently to keep up-to-date with the remote:

```bash
git pull
make -j N
```

Since MOOSE is included as a submodule of RACCOON, you will receive a notification when a new version of MOOSE is available for use. To update MOOSE, simply do

```bash
git submodule update
conda activate moose
conda update --all
```

!alert note title= updating the conda environment
Since MOOSE is built upon libMesh, updating the conda environment packages including `moose-libmesh` without updating MOOSE may break dependencies.

## 7. Set up Atom for development (Optional)

We recommend Atom as the IDE for RACCOON (or in general any MOOSE-based applications). Follow [these instructions](install/ide.md) to set up Atom for rapid development.

## 8. Install ParaView for visualization (Optional)

The standard output file of RACCOON is Exodus, a file format developed by the Sandia National Laboratory. [ParaView](https://www.paraview.org/) is a powerful visualization toolkit and complies with the Exodus file format.

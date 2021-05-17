# Install RACCOON

RACCOON is a perfect example of integration of multiple open source projects.

- RACCOON is built upon [MOOSE](https://mooseframework.inl.gov/), an object oriented parallel FEM framework.
- MOOSE is built upon [libMesh](http://libmesh.github.io/), a C++ FEM library, to utilize its FEM basics.
- Both MOOSE and libMesh rely on many other tools, such as [PETSc](https://www.mcs.anl.gov/petsc/).

Therefore, it can be complicated to get RACCOON to work. Fortunately, the installation process
has been thought through and thoroughly tested. The installation process can be summarized in the following steps,
each of which should only take a handful of commands.

!alert note title= Install RACCOON on the Duke Computing Cluster (DCC)
Instead of the instructions below, refer to [these instructions](https://github.com/casperversteeg/fuzzy-pancake-eureka/blob/master/DCCmoose.pdf) for setting up RACCOON on the Duke Computing Cluster. Contact [Casper Versteeg](mailto:casper.versteeg@duke.edu) if you need access to these instructions. Contact Professor [John Dolbow](mailto:jdolbow@duke.edu) if you are our group member and need access to DCC.

## Step 1: Install environment packages style=line-height:150%;

First, follow [these instructions](getting_started/conda.md) to install environment packages for MOOSE and RACCOON.

## Step 2: Clone RACCOON style=line-height:150%;

Next, clone RACCOON to your local projects directory:

```bash
mkdir ~/projects
cd ~/projects
git clone https://github.com/hugary1995/raccoon.git
cd raccoon
git checkout master
git submodule update --init --recursive
```

These commands should download a copy of RACCOON and a copy of MOOSE (as a submodule) to your local projects directory.

> \[Optional\] If you didn't choose to include moose-libmesh in your Conda environment, you need to compile libMesh using
>
> ```bash
> ./moose/scripts/update_and_rebuild_libmesh.sh
> ```

## Step 3: Compile RACCOON style=line-height:150%;

Next, you can compile RACCOON using

```bash
make -j N
```

where `N` is the number of processors you want to use to compile RACCOON in parallel.

> \[Optional\] To make sure RACCOON is working properly, run the regression tests:
>
> ```bash
> ./run_tests -j N
> ```

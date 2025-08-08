# Install RACCOON

RACCOON is a perfect example of integration of multiple open source projects.

- RACCOON is built upon [MOOSE](https://mooseframework.inl.gov/), an object oriented parallel FEM framework.
- MOOSE is built upon [libMesh](http://libmesh.github.io/), a C++ FEM library, to utilize its FEM basics.
- Both MOOSE and libMesh rely on many other tools, such as [PETSc](https://www.mcs.anl.gov/petsc/).

Therefore, it can be complicated to get RACCOON to work. Fortunately, the installation process
has been thought through and thoroughly tested. The installation process can be summarized in the following steps,
each of which should only take a handful of commands.

!include getting_started/dcc.md

### Step 1: Install dependencies style=line-height:150%

First, follow [these instructions](https://mooseframework.inl.gov/getting_started/installation/conda.html) to install environment packages for MOOSE and RACCOON. Stop at the header "Cloning MOOSE".

### Step 2: Clone RACCOON style=line-height:150%

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

> +\[Optional\]+ In Step 1, if you didn't choose to include moose-libmesh in your Conda environment, you need to compile PETSc and libMesh using
>
> ```bash
> ./moose/scripts/update_and_rebuild_petsc.sh
> ./moose/scripts/update_and_rebuild_libmesh.sh
> ```

### Step 3: Compile RACCOON style=line-height:150%

Next, you can compile RACCOON using

```bash
make -j N
```

where `N` is the number of processors you want to use to compile RACCOON in parallel.

> +\[Optional\]+ To make sure RACCOON is working properly, run the regression tests:
>
> ```bash
> ./run_tests -j N
> ```

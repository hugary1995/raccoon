# Install

RACCOON is a perfect example of integration of multiple open source projects.

- RACCOON is built upon [MOOSE](https://mooseframework.inl.gov/), an object oriented parallel FEM framework.
- MOOSE is built upon [Libmesh](http://libmesh.github.io/), a C++ FEM library, to utilize its FEM basics.
- Both MOOSE and Libmesh rely on many other tools, such as [PETSC](https://www.mcs.anl.gov/petsc/).

Therefore, it can be complicated to get RACCOON to work. Fortunately, the installation process
has been thought through and thoroughly tested. The installation process can be summarized in the following steps,
each of which should only take a handful of commands:

- Install environment packages
- Clone RACCOON
- Compile RACCOON
- Test RACCOON

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

## 3. Compile RACCOON

```bash
make -j N
```

where `N` is the number of processors you want to use to compile RACCOON in parallel.

## 4. Test RACCOON

To make sure RACCOON is working properly, run the script `run_tests` to automatically run all regression tests:

```bash
./run_tests -j N
```

Optionally, you may try [benchmark problems](benchmarks/index.md).

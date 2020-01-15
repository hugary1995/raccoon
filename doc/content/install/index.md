# Install

RACCOON is a perfect example of integration of multiple open source projects.

- RACCOON is built upon [MOOSE](https://mooseframework.inl.gov/), an object oriented parallel FEM framework.
- MOOSE is built upon [Libmesh](http://libmesh.github.io/), a C++ FEM library, to utilize its FEM basics.
- Both MOOSE and Libmesh rely on many other tools, such as [PETSC](https://www.mcs.anl.gov/petsc/).

Therefore, it can be complicated to get RACCOON to work. Fortunately, the installation process
has been thought through and thoroughly tested. The installation process can be summarized in the following steps,
each of which should only take a handful of commands:

- Install environment packages
- Build MOOSE
- Acquire a copy of RACCOON
- Compile RACCOON
- Test RACCOON

## Install environment packages

For most up-to-date package dependencies, please go to the [MOOSE](https://mooseframework.inl.gov/getting_started/index.html) website for latest instructions.
Choose a redistributable package based on your operation system and follow rest of the instructions there.

## Build MOOSE

Note that RACCOON is built upon a fork of MOOSE, hence, when you are asked to obtain a copy of MOOSE, make sure to clone from the following repository instead of the upstream idaholab copy:

```bash
git clone https://github.com/hugary1995/moose.git
cd moose
git checkout next
```

## Acquire a copy of RACCOON

RACCOON is also hosted on Github open source. First clone RACCOON to your local projects directory.

```bash
cd ~/projects
git clone https://github.com/hugary1995/raccoon.git
cd raccoon
git checkout master
```

## Setup environment variables

RACCOON assumes two environment variables `MOOSE_DIR` and `RACCOON_DIR`. To set `MOOSE_DIR` for example, navigate to the MOOSE directory and type pwd:

```bash
cd ~/projects/moose
pwd
```

Output of the `pwd` command should look like:

```bash
/home/gary/projects/moose
```

Then set the output of the `pwd` command as your `MOOSE_DIR` environment variable by appending the following line to ~/.bashrc (or ~/.bash_profile for MacOS)

```bash
export MOOSE_DIR=/home/gary/projects/moose
```

Then close all the terminals to make sure the environment variable is in effect. Test if the environment variable is set by typing:

```bash
echo $MOOSE_DIR
```

The same steps should be followed to set the environment variable for `RACCOON_DIR` which points to `~/projects/raccoon`.

## Compile RACCOON

After all MOOSE tests complete, RACCOON can be compiled by

```bash
cd ~/projects/raccoon
make -j N
```

where `N` is the number of processors you want to use to compile RACCOON in parallel

## Test RACCOON

To make sure RACCOON is working properly, run the script `run_tests` to automatically run all regression tests:

```bash
cd ~/projects/raccoon
./run_tests -j N
```

Optionally, you may try [benchmark problems](benchmarks/index.md).

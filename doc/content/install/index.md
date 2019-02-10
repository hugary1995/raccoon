# Install

RACCOON is a perfect example of integration of multiple open source projects.

- RACCOON is built upon [MOOSE](https://mooseframework.inl.gov/), an object oriented parallel FEM framework.
- MOOSE is built upon [Libmesh](http://libmesh.github.io/), a C++ FEM library, to utilize its FEM basics.
- Both MOOSE and Libmesh rely on external numerical solvers such as [PETSC](https://www.mcs.anl.gov/petsc/).

Therefore, it can be complicated to get RACCOON to work. Fortunately, the installation process
has been thought through and thoroughly tested. The installation process can be summarized in the following steps,
each of which should only take a handful of commands:

- Build MOOSE

  - Install prerequisite packages
  - Clone MOOSE
  - Build a MOOSE-compatible PETSC
  - Compile MOOSE with PETSC
  - Test MOOSE
- Acquire a copy of RACCOON
- Compile RACCOON
- Test RACCOON

## Build MOOSE

For most up-to-date package dependencies, please go to [MOOSE](https://mooseframework.inl.gov/getting_started/index.html) website for latest instructions.
Choose a redistributable package based on your operation system and follow rest of the instructions there.

## Acquire a copy of RACCOON

RACCOON is also hosted on Github open source. First clone RACCOON to your local projects directory.

```bash
cd ~/projects
git clone https://github.com/hugary1995/raccoon.git
cd raccoon
git checkout master
```

## Compile RACCOON

After all MOOSE tests complete, make sure `MOOSE_DIR` environment variable is set so that RACCOON is aware of the location of MOOSE.
For example, navigate to the MOOSE directory and type pwd:

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

Now go back to RACCOON and compile:

```bash
cd ~/projects/raccoon
make -j N
```

where `N` is the number of processors you want to use to compile RACCOON in parallel

## Test RACCOON

To make sure RACCOON is working properly, run the script `run_tests` to automatically run all unit tests and exodiff the outputs:

```bash
cd ~/projects/raccoon
./run_tests -j N
```

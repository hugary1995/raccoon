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

## 1.1 Install environment packages (The old way)

For most up-to-date package dependencies, please go to the [MOOSE](https://mooseframework.inl.gov/getting_started/index.html) website for latest instructions.
Choose a redistributable package based on your operation system and follow rest of the instructions there. Then, you may skip down to [Build MOOSE](install/index.md#buildmoose)

## 1.2 Install environment packages (The easy way)

Our preferred method for obtaining libraries necessary for MOOSE based Application development, is through Conda. Follow these instructions to create an environment on your machine using Conda. At this time, a Conda install via Windows is not supported.

### 1.2.1 Transitional step for pre-existing users

For those of you who have previously installed the moose-environment package, you should remove it. +If you are a first time MOOSE user, please skip down to [Install Conda](install/index.md#installconda).+ Removal of the moose-envirnment package, is only needed to be performed once.

- Using Conda, it is no longer neccessary to have /opt/moose present on your machine. Depending on the type of machine you have, please do the following:

  | Operating System | Command |
  | :- | -: |
  | CentOS | `sudo yum remove moose-environment` |
  | Fedora | `sudo dnf remove moose-environment` |
  | OpenSUSE | `sudo zypper remove moose-environment` |
  | Debian (Ubuntu, Mint) | `sudo dpkg -r moose-environment` |
  | Macintosh | `sudo rm -rf /opt/moose` |

!alert warning title= sudo is dangerous
Be especially carful with the above commands! Verify +twice+ that what you have entered in your terminal is what the instructions are asking you to do.

### 1.2.2 Configure the MOOSE environment id=installconda

- [Install Miniconda or Anaconda](https://docs.conda.io/projects/conda/en/latest/user-guide/install/index.html) (we recommend Miniconda)

- Configure Conda to work with conda-forge, and our Idaholab channel:

  ```bash
  conda config --add channels conda-forge
  conda config --add channels https://mooseframework.org/conda/moose
  ```

- Install the moose-env package from Idaholab and name your environment 'moose':

  ```bash
  conda create --name moose moose-env
  ```

- Activate the moose environment +(do this for any new terminal opened)+:

  ```bash
  conda activate moose
  ```

Some folks may receive additional instructions when attempting to activate a profile. Follow those instructions, and try to activate the moose environment again.

You will have successfully activated the moose environment when you see 'moose' within your prompt.

If you close, and then re-open this terminal window, know that you will need to `conda activate moose` again. You will need to do this for every terminal you open. If you wish to make this automatic, you can append `conda activate moose` to your bash or zsh profile.

The MOOSE team will make periodic updates to the conda packages. To stay up-to-date, activate the moose environment, and perform an update:

```bash
conda activate moose
conda update --all
```

!alert note title= sudo is not necessary
If you find yourself applying the use of `sudo` for any of the above conda commands... something's not right. The most common reason for needing sudo, is due to an improper installation. Conda *should* be installed to your home directory, and without any use of `sudo`.

## 2. Build MOOSE id=buildmoose

Note that RACCOON is built upon a fork of MOOSE, hence, when you are asked to obtain a copy of MOOSE, make sure to clone from the following repository instead of the upstream idaholab copy:

```bash
git clone https://github.com/hugary1995/moose.git
cd moose
git checkout next
```

## 3. Acquire a copy of RACCOON

RACCOON is also hosted on Github open source. First clone RACCOON to your local projects directory.

```bash
cd ~/projects
git clone https://github.com/hugary1995/raccoon.git
cd raccoon
git checkout master
```

## 4. Setup environment variables

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

## 5. Compile RACCOON

After all MOOSE tests complete, RACCOON can be compiled by

```bash
cd ~/projects/raccoon
make -j N
```

where `N` is the number of processors you want to use to compile RACCOON in parallel

## 6. Test RACCOON

To make sure RACCOON is working properly, run the script `run_tests` to automatically run all regression tests:

```bash
cd ~/projects/raccoon
./run_tests -j N
```

Optionally, you may try [benchmark problems](benchmarks/index.md).

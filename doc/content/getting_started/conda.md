# Install MOOSE environment

!alert note title= Toubleshooting Conda
If you have any issue with Conda, first read the [troubleshooting guide for Conda](https://mooseframework.inl.gov/help/troubleshooting.html#condaissues).

First, install Miniconda if you haven't already:

- +Linux:+

  ```bash
  curl -L -O https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
  bash Miniconda3-latest-Linux-x86_64.sh -b -p ~/miniconda3
  ```

- +Macintosh:+

  ```bash
  curl -L -O https://repo.anaconda.com/miniconda/Miniconda3-latest-MacOSX-x86_64.sh
  bash Miniconda3-latest-MacOSX-x86_64.sh -b -p ~/miniconda3
  ```

With Miniconda installed to your home directory, export PATH, so that it may be used:

```bash
export PATH=$HOME/miniconda3/bin:$PATH
```

Configure Conda to work with conda-forge and the idaholab channel:

```bash
conda config --add channels conda-forge
conda config --add channels https://conda.software.inl.gov/public
```

!alert warning title= sudo is not necessary
If you find yourself applying the use of `sudo` for any of the above conda commands... something's not right. The most common reason for needing sudo, is due to an improper installation. Conda *should* be installed to your home directory, and without any use of `sudo`.

Now, you have a choice to make:

- +\[Option 1\] (Recommended) Use compiled libMesh from Conda+

  Use this option if you don't need to do libMesh development. If you are unsure about that, then proceed with this option. Install the moose-libmesh and moose-tools package, and name your environment 'moose':

  ```bash
  conda create --name moose moose-libmesh moose-tools
  ```

- +\[Option 2\] Configure your own libMesh+

  In case you want to do developments with libMesh, it may be convenient to compile your own libMesh. In this case, only install the moose-tools package, and name your environment 'moose':

  ```bash
  conda create --name moose moose-tools
  ```


Then you can activate the moose environment +(do this for any new terminal opened)+:

```bash
conda activate moose
```

!alert note title= First time using conda
Some folks may receive additional instructions when attempting to activate a profile. Follow those instructions, and try to activate the moose environment again. You will have successfully activated the moose environment when you see the environment name within your prompt.

!alert note title= Activate conda environment for every ner terminal
If you close, and re-open the terminal window, know that you will need to activate the environment again. You will need to do this for each terminal you open. If you wish to make this automatic, you may append `conda activate moose` to your shell profiles.

Next, head back to the installation instructions and continue from Step 2.

!content pagination use_title=True
                    previous=getting_started/install.md
                    next=getting_started/install.md

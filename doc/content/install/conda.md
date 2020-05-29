Currently, only Linux and MacOS builds are being actively tested. Follow these instructions to create an environment on your machine using Conda. At this time, an option to install MOOSE directly on a Windows system is not yet supported.

!alert note title= Anaconda also works
You may choose to use Anaconda if you so desire: [Anaconda for Python 3.x](https://www.anaconda.com/distribution/)

You may follow Miniconda's instructions: [Install Miniconda3](https://docs.conda.io/en/latest/miniconda.html), however we continuously receive reports of difficulty doing so (especially from our MacOS users). With that said, please use our instructions to install Miniconda instead:

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

Configure Conda to work with conda-forge, and our mooseframework.org channel:

```bash
conda config --add channels conda-forge
conda config --add channels https://mooseframework.org/conda/moose
```

- +\[Option 1\] Use compiled libMesh from Conda+

  Install the moose-libmesh and moose-tools package from mooseframework.org, and name your environment 'moose':

  ```bash
  conda create --name moose moose-libmesh moose-tools
  ```

  Activate the moose environment +(do this for any new terminal opened)+:

  ```bash
  conda activate moose
  ```

  Some folks may receive additional instructions when attempting to activate a profile. Follow those instructions, and try to activate the moose environment again.

  You will have successfully activated the moose environment when you see 'moose' within your prompt.

  If you close, and re-open this terminal window, know that you will need to `conda activate moose` again. You will need to do this for each terminal you open. If you wish to make this automatic, you may append `conda activate moose` to your bash or zsh profiles.

  The MOOSE team will make periodic updates to the conda packages. To stay up-to-date, activate the moose environment, and perform an update:

  ```bash
  conda activate moose
  conda update --all
  ```

- +\[Option 2\] Configure your own Conda+

  In case you want to do developments with libMesh as well, if may be convenient to compile your own libMesh.

  As a demonstration, we show how to configure your own libMesh with Boost.
  In this case, install the moose-tools and moose-petsc packages from mooseframework.org, install boost package from conda-forge, and name your environment 'moose-boost':

  ```bash
  conda create --name moose-boost moose-petsc moose-tools boost
  ```

  Activate the moose environment +(do this for any new terminal opened)+:

  ```bash
  conda activate moose-boost
  ```

  Some folks may receive additional instructions when attempting to activate a profile. Follow those instructions, and try to activate the moose environment again.

  You will have successfully activated the moose environment when you see 'moose-boost' within your prompt.

  If you close, and re-open this terminal window, know that you will need to `conda activate moose-boost` again. You will need to do this for each terminal you open. If you wish to make this automatic, you may append `conda activate moose-boost` to your bash or zsh profiles.

  The MOOSE team will make periodic updates to the conda packages. To stay up-to-date, activate the moose environment, and perform an update:

  ```bash
  conda activate moose-boost
  conda update --all
  ```




!alert note title= sudo is not necessary
If you find yourself applying the use of `sudo` for any of the above conda commands... something's not right. The most common reason for needing sudo, is due to an improper installation. Conda *should* be installed to your home directory, and without any use of `sudo`.

Now, head back over to the [install](install/index.md) page to continue your journey of RACCOON.

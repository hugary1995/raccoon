# Update RACCOON

RACCOON is under rapid development right now. To make sure you get the most out of RACCOON, update your Conda environment as well as your local repository frequently to keep up-to-date with the remote.

To update the Conda environment:

```bash
conda activate moose
conda update --all
```

To update RACCOON:

```bash
git pull
git submodule update
make -j N
```

!alert note title=RACCOON is tracked by MOOSE
RACCOON is tracked by MOOSE so the submodule moose in the raccoon repository is (almost) always in sync with idaholab/moose.

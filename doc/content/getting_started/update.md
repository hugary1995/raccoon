# Update RACCOON

RACCOON is under rapid development right now. To make sure you get the most out of RACCOON, update your Conda environment as well as your local repository frequently to keep up-to-date with the remote.

To update the Conda environment, use

```bash
conda activate moose
conda update --all
```

To update RACCOON, use

```bash
git pull
git submodule update --init --recursive
```

Then, recompile RACCOON using

```bash
make -j N
```

> Sometimes after updating RACCOON and all submodules, you may get compile errors. Most likely that is due to stale objects floating around in the repository. To clean stale objects, +make sure you have everything committed+, and do
>
> ```bash
> git submodule foreach --recursive git clean -xfd
> git clean -xfd
> ```
>
> After which you should be able to recompile RACCOON
>
> ```bash
> make -j N
> ```

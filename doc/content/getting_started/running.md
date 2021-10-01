# Running your first model

## Understanding the code organization

Before running any model, it is important to understand what a "model" means in MOOSE. In the MOOSE ecosystem, the source code and the configuration of your model are separated.

The source codes of your model are written in C++, and they are compiled into an executable file. In RACCOON the executable is called `raccoon-opt`.

The configurations of your model are written in Hit format, and they are called "input files" with a `.i` extension. While your models may work for a wide variety of problems, an input file typically contains configurations for a *specific* problem. An input file may contain information about mesh, variables, weak form, constitutive models, boundary conditions, initial conditions, postprocessors, and so on.

> For example, a very general constitutive model for [isotropic linear elasticity](SmallDeformationIsotropicElasticity.md) is implemented in RACCOON:
>
> - Its source code is written in C++: [SmallDeformationIsotropicElasticity.C](SmallDeformationIsotropicElasticity.C),
> - together with its header file: [SmallDeformationIsotropicElasticity.h](SmallDeformationIsotropicElasticity.h)
>
> This constitutive model calculates the stress given the strain, the bulk modulus, and the shear modulus. However, =the bulk modulus and the shear modulus need not be hard-coded= in the source code. The values of the bulk modulus and the shear modulus are retrieved from an input file. In an input file, this constitutive model is used with the following syntax:
>
> !listing tutorials/small_deformation/elasticity.i
>          block=Materials/elasticity
>          language=python

!alert tip title=No hard-coding!
We highly recommend you to follow this idea of separating the model from its parameters. As this allows for more general code, less code duplication, maximum modularity/reusability, and minimum maintenance cost.

## Command line interface

Before running a model, first make sure you have installed and compiled RACCOON following [the installation guide](install.md).

To run a model, go to the directory of its input files, e.g.

```bash
cd ~/projects/raccoon/tutorials/small_deformation
```

and run the input file using the compiled executable:

```bash
../../raccoon-opt -i elasticity.i
```

The first argument is the path to the RACCOON executable. It is followed by a `-i` option meaning that we will supply our input file immediately after. `elasticity.i` is the path to the input file that we want to run.

To view the complete list of options, simply run the executable without any additional arguments:

```bash
../../raccoom-opt
```

To run the model using multiple processors in parallel, use `mpiexec`, e.g.

```bash
mpiexec -n 2 ../../raccoon-opt -i elasticity.i
```

where 2 is the number of processors you want to use.

After the calculation is finished, you can visualize the result following [the visualization guide](getting_started/paraview.md)

## Where to store your own models?

Since the implementation and the configuration of a model are separated, we need to consider them separately:

- The source code of the model, i.e. `.C` files, should be stored in `~/projects/raccoon/src`.
- The header file of the model, i.e. `.h` files, should be stored in `~/projects/raccoon/include`.
- The input files and all of their accompanying files shall be stored in a separate repository outside RACCOON, unless you want to eventually merge into the official RACCOON repository.

For your convenience, there is a symlink in the RACCOON repository to a directory sitting in parallel. The symlink is

```bash
~/projects/raccoon/examples -> ~/projects/raccoon_examples
```

meaning that if you create a directory outside RACCOON called `raccoon_examples`, you will be able to access of its content from `~/projects/raccoon/examples` as if they sit right there, but all content within that directory are not under version control (you can still set up one on your own, and it is actually highly recommended).

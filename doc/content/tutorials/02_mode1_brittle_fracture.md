!include tutorials/tutorial_header.md

# Tutorial 2: Mode-I crack propagation

In this tutorial, we will set up the model for Mode-I crack propagation.

The geometry and boundary conditions are the same as those in [Tutorial 1](tutorials/01_small_deformation_elasticity.md).

!media media/mode1_bcs.png alt=mode1_bcs style=display:block;margin:auto;width:60%; caption=Geometry and boundary conditions of the Mode-I crack propagation problem. id=mode1_schematics

At each time step, the alternative minimization scheme first solves the displacements with a fixed phase-field, then solves the phase-field with the updated displacements. This scheme is realized using MOOSE's MultiApp system. We will set up two input files. One for the displacement subproblem, and the other one for the phase-field subproblem.

## `elasticity.i`: The displacement subproblem

First, we set up the displacement subproblem as our *main app*. Two additional global expressions are defined at the top of the input file:

!listing tutorials/mode1_brittle_fracture/elasticity.i
         start=Gc
         end=l
         include-end=True
         link=False
         language=python

The mesh is locally refined using initial adaptivity:

!listing tutorials/mode1_brittle_fracture/elasticity.i
         block=Adaptivity
         link=False
         language=python

The quadratic degradation function is defined as

!listing tutorials/mode1_brittle_fracture/elasticity.i
         block=Materials/degradation
         link=False
         language=python

## `fracture.i`: The phase-field subproblem

Next, we set up the phase-field subproblem as our *sub app*. The sub app uses the same mesh as the main app. The equation we want to solve is

\begin{equation}
  - \divergence \dfrac{2 \Gc l}{c_0} \grad d + \dfrac{\partial}{\partial d} \left( \alpha \dfrac{\Gc}{c_0l} + \psi^e \right) = 0,
\end{equation}

on the inactive set. The (local part of the) free energy, i.e. the term in the paranthesis, is defined using an `ADDerivativeParsedMaterial`:

!listing tutorials/mode1_brittle_fracture/fracture.i
         block=Materials/psi
         link=False
         language=python

With the definition of local free energy `psi`, the following kernels are used to construct the equation:

!listing tutorials/mode1_brittle_fracture/fracture.i
         block=Kernels
         link=False
         language=python

The bound constraints $d_{n-1} \leqslant d_n \leqslant 1$ on the active set is defined using

!listing tutorials/mode1_brittle_fracture/fracture.i
         block=Bounds
         link=False
         language=python

## Transferring information between the main and the sub app

The displacement subproblem need to know the solution of the phase-field subproblem, and the phase-field subproblem need to know the active elastic energy `psie_active`. Therefore, some transfer of information between the two apps must exist.

To do that, we first let the displacement subproblem (the main app) be aware of its subapp by adding the following section to `elasticity.i`:

!listing tutorials/mode1_brittle_fracture/elasticity.i
         block=MultiApps
         link=False
         language=python

The `cli_args` makes sure $\Gc$ and $l$ are consistent across the two apps. The `execute_on` flag tells the main app to execute the sub app at the end of the time step (after the displacements are solved).

Next, we set up the appropriate transfer:

!listing tutorials/mode1_brittle_fracture/elasticity.i
         block=Transfers
         link=False
         language=python

The phase-field $d$ is retrieved from the sub app, and the main app sends the active elastic energy $psie_active$ to the sub app.

## The alternating minimization scheme

Now that we have set up the displacement subproblem, the phase-field subproblem, and the transfers between them, we are now in good shape to set up the alternating minimization scheme.

To do so, we rely on MOOSE's Picard iteration. In essence, a Picard solve iterates between the main app and the sub app(s) until the solution in the main app no longer changes between Picard iterations. We add the following parameters to the `Executioner` section to enable Picard iteration:

!listing tutorials/mode1_brittle_fracture/elasticity.i
         start=picard_max_its
         end=picard_abs_tol
         include-end=True
         link=False
         language=python

## The complete input files

- [elasticity.i](tutorials/mode1_brittle_fracture/elasticity.i)
- [fracture.i](tutorials/mode1_brittle_fracture/fracture.i)

!content pagination previous=tutorials/01_small_deformation_elasticity.md
                    next=tutorials/03_mode2_brittle_fracture.md

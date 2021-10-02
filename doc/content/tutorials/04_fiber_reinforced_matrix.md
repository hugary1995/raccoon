!include tutorials/tutorial_header.md

# Tutorial 4: Fiber-reinforced matrix

In this tutorial, we will set up the model for Fiber-reinforced matrix.

Consider a two-dimensional square plate with a fiber in the center under stretch. We will set up two input files. One for the displacement subproblem, and the other one for the phase-field subproblem.

## `elasticity.i`: The displacement subproblem

First, we set up the displacement subproblem as our *main app*. Global expressions are defined at the top of the input file:

!listing tutorials/matrix_fiber/elasticity.i
         start=E
         end=v
         include-end=True
         link=False
         language=python

The mesh file:

!listing tutorials/matrix_fiber/elasticity.i
         block=Mesh
         link=False
         language=python

The quadratic degradation function is defined as

!listing tutorials/matrix_fiber/elasticity.i
         block=Materials/degradation
         link=False
         language=python

## `fracture.i`: The phase-field subproblem

The equation we want to solve is

\begin{equation}
  - \divergence \dfrac{2 \Gc l}{c_0} \grad d + \dfrac{\partial}{\partial d} \left( \alpha \dfrac{\Gc}{c_0l} + \psi^e \right) = 0,
\end{equation}

on the inactive set. The (local part of the) free energy, i.e. the term in the paranthesis, is defined using an `ADDerivativeParsedMaterial`:

!listing tutorials/matrix_fiber/fracture.i
         block=Materials/psi
         link=False
         language=python

With the definition of local free energy `psi`, the following kernels are used to construct the equation:

!listing tutorials/matrix_fiber/fracture.i
         block=Kernels
         link=False
         language=python

The bound constraints $d_{n-1} \leqslant d_n \leqslant 1$ on the active set is defined using

!listing tutorials/matrix_fiber/fracture.i
         block=Bounds
         link=False
         language=python

## Transferring information between the main and the sub app

The displacement subproblem need to know the solution of the phase-field subproblem, and the phase-field subproblem need to know the active elastic energy `psie_active`. Therefore, some transfer of information between the two apps must exist.

To do that, we first let the displacement subproblem (the main app) be aware of its subapp by adding the following section to `elasticity.i`:

!listing tutorials/matrix_fiber/elasticity.i
         block=MultiApps
         link=False
         language=python

The `cli_args` makes sure $\Gc$ and $l$ are consistent across the two apps. The `execute_on` flag tells the main app to execute the sub app at the end of the time step (after the displacements are solved).

Next, we set up the appropriate transfer:

!listing tutorials/matrix_fiber/elasticity.i
         block=Transfers
         link=False
         language=python

The phase-field $d$ is retrieved from the sub app, and the main app sends the active elastic energy `psie_active` to the sub app.

## The alternating minimization scheme

Now that we have set up the displacement subproblem, the phase-field subproblem, and the transfers between them, we are now in good shape to set up the alternating minimization scheme.

!listing tutorials/matrix_fiber/elasticity.i
         block=Executioner
         link=False
         language=python

## The complete input files

- [elasticity.i](tutorials/matrix_fiber/elasticity.i)
- [fracture.i](tutorials/matrix_fiber/fracture.i)

!content pagination previous=tutorials/03_matrix_fiber.md
                    next=tutorials/05_soil_desiccation.md

!include tutorials/tutorial_header.md

# Tutorial 1: Small deformation elasticity

This tutorial covers the basic usage of RACCOON.

Consider a two-dimensional square plate with a notch (the commonly used geometry for Mode-I fracture) under stretch, we would like to solve for the displacements and visualize its strains and stresses everywhere.

!media media/mode1_bcs.png style=display:block;margin:auto;width:60%; caption=Geometry and boundary conditions of the Mode-I crack propagation problem. id=mode1_schematics

## Global expressions and parameters

It is always good practice to define parameters using expressions at the top of the input file. Here, we first define two expressions for Young's modulus and Poisson's ratio:

!listing tutorials/small_deformation/elasticity.i
         start=E
         end=nu
         include-end=True
         link=False
         language=python

Next, we convert $E$ and $\nu$ to bulk modulus $K$ and shear modulus $G$ using brace expressions:

!listing tutorials/small_deformation/elasticity.i
         start=K
         end=G
         include-end=True
         link=False
         language=python

For our convenience, let's also define the global parameter `displacements` as that is required by several objects, e.g. `ADStressDivergenceTensors`, `ADComputeSmallStrain`, etc.. `GlobalParams` definitions will be applied wherever applicable throughout the input file, unless locally overridden.

!listing tutorials/small_deformation/elasticity.i
         block=GlobalParams
         link=False
         language=python

## Mesh

Let's utilize symmetry about the x-axis so that we can get away with only modeling the top half of the domain:

!listing tutorials/small_deformation/elasticity.i
         block=Mesh
         link=False
         language=python

Notice that we defined an extra nodeset/sideset called "noncrack", so that we can later apply the $u_y = 0$ symmetry condition.

## Variables and AuxVariables

In this problem we solve for displacements in the x- and y- directions, hence we define

!listing tutorials/small_deformation/elasticity.i
         block=Variables
         link=False
         language=python

In addition, we define auxiliary variables `fy` and `d`:

!listing tutorials/small_deformation/elasticity.i
         block=AuxVariables
         link=False
         language=python

The AuxVariable `fy` will store residuals associated with Variable `disp_y`. Later, we will use it to compute the reaction force.

The AuxVariable `d` is a dummy variable representing the phase-field that is zero everywhere. In this tutorial, it will stay zero.

## Kernels

Kernels are arguably the most important objects in an input file. They define the equations we are solving for. In this case, we solve for

\begin{equation}
  \divergence \stress = \bs{0},
\end{equation}
without any body force.

Kernels are defined as

!listing tutorials/small_deformation/elasticity.i
         block=Kernels
         link=False
         language=python

We are solving a vector-valued equation, and the two kernels correspond to the two components, respectively.

## Boundary conditions

Boundary conditions are shown in [mode1_schematics]: only the top half of the domain is modeled utilizing symmetry. On the bottom of the computational domain, i.e. a nodeset named "noncrack" is generated and used to define the symmetry condition. A displacement-controlled Dirichlet boundary condition is applied on the top. In the input file, these boundary conditions correspond to

!listing tutorials/small_deformation/elasticity.i
         block=BCs
         link=False
         language=python

The y-displacement on the top boundary is set to be equal to the time `t`. In this quasi-static setting, we can use time as a convenient dummy load-control parameter.

## Material models

The material is assumed to be isotropic elastic under small strain assumptions. First, the homogeneous shear and bulk modulus are defined everywhere (at every quadrature point):

!listing tutorials/small_deformation/elasticity.i
         block=Materials/bulk
         link=False
         language=python

Since we don't consider any fracture coupling, a dummy `NoDegradation` degradation function is defined as

!listing tutorials/small_deformation/elasticity.i
         block=Materials/no_degradation
         link=False
         language=python

Then, the `Materials` section is completed by the definition of the strain calculator, the constitutive relation, and the stress calculator:

!listing tutorials/small_deformation/elasticity.i
         block=Materials
         link=False
         language=python

Notice that we have requested to output `elastic_strain` and `stress` to exodus. Since they are `RankTwoTensor`s, we will find elemental variables with names like `elastic_strain_00` and `stress_22`, denoting their corresponding component.

!alert tip title=Material evaluation
It is imperative to understand that, in MOOSE, `MaterialProperties` are computed *on-thy-fly*. Material dependency is automatically resolved by MOOSE.

## Postprocessors

Finally, we add a postprocessor to sum the nodal values of `fy` along the top boundary to give us the reaction force in the y-direction:

!listing tutorials/small_deformation/elasticity.i
         block=Postprocessors
         link=False
         language=python

## [The complete input file](tutorials/small_deformation/elasticity.i)

!content pagination next=tutorials/02_mode1_brittle_fracture.md

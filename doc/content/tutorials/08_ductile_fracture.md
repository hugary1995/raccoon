!include tutorials/tutorial_header.md

# Tutorial 8: Ductile fracture

!content pagination previous=tutorials/07_elastoplasticity.md
next=tutorials/09_three_point_bending.md
This tutorial covers the implementation of ductile fracture

For simplicity, we will consider a single element 3 dimensional homogeneous cube under tension. For this tutorial we will apply J2 plasticity with a linear hardening model, but in the tutorial folder you will find examples with other configurations.

## `elastoplasticity.i`: The displacement subproblem

Similar to previous tutorials, we need to define parameters.
We define additional parameters related to plasticity.

!listing tutorials/homogeneous_cube/Hencky_J2_linearhardening/elastoplasticity.i
  start=E
  end=H
  include-end=True
  link=False
  language=python

Where `H` is hardening modulus and `sigma_y` is the yield stress.


  `GeneratedMeshGenerator` is utilized to create a 3 dimensional cube.

!listing tutorials/homogeneous_cube/Hencky_J2_linearhardening/elastoplasticity.i
    block=Mesh
    link=False
    language=python


We are now transferring an additional variable to the fracture multi-app, psip_active. This is the active plastic energy. In this model active plastic energy contributes to the fracture driving energy.

!listing tutorials/homogeneous_cube/Hencky_J2_linearhardening/elastoplasticity.i
    block=Transfers
    link=False
    language=python



We have an additional auxiliary variable defined, `F`. This is a consequence of utilizing large deformation models. Previous tutorials utilized small deformation models.

!listing tutorials/homogeneous_cube/Hencky_J2_linearhardening/elastoplasticity.i
    block=AuxVariables
    link=False
    language=python

Plasticity is added through constitutive updates in the Materials block.

!listing tutorials/homogeneous_cube/Hencky_J2_linearhardening/elastoplasticity.i
    block=Materials
    link=False
    language=python

Here we add the blocks for `HenckyIsotropicElasticity`, `linearhardening`, `LargeDeformationJ2Plasticity`, and `ComputeLargeDeformationStress`.

`LargeDeformationJ2Plasticity`: This utilizes the selected hardening model to calculate the value of the yield surface and determine whether the material has begun plastic deformation.

`HenckyIsotropicElasticity:` This calculates elastic stress using the hencky model. Returns the Mandel stress to `ComputeLargeDeformationStress`

`LinearHardening:`This performs plastic hardening following a linear isotropic law. It returns plastic energy to `LargeDeformationJ2Plasticity`

`ComputeLargeDeformationStress:`This computes stress given an elasticity model and a plasticity model. It uses the elasticity model to compute Cauchy stress and the plasticity model to update the plastic deformation to bring the stress state back unto the yield surface.

## Modifications

The elasticity model and hardening law used can be changed.

Currently available hyperelasticity models

1. `HenckyIsotropicElasticity`
2. `CNHIsotropicElasticity`

Currently available hardening models

1. `ArrheniusLawHardening`
2. `LinearHardening`
3. `PowerLawHardening`


## `fracture.i`: The phase-field subproblem

There are two minor changes to be made to the fracture input file seen in previous tutorials.

First, we add an auxiliary variable for active plastic energy. This is to receive the value from the main app.

!listing tutorials/homogeneous_cube/Hencky_J2_linearhardening/fracture.i
    block=AuxVariables
    link=False
    language=python



Second, we add the active plastic energy into the fracture driving energy

!listing tutorials/homogeneous_cube/Hencky_J2_linearhardening/fracture.i
    block=psi
    link=False
    language=python


## Alternating Minimization

Like in previous tutorials, we rely on on MOOSE's Picard iteration to create an alternating minimization scheme.

!listing tutorials/homogeneous_cube/Hencky_J2_linearhardening/fracture.i
    block=Executioner
    link=False
    language=python

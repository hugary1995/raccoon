!include tutorials/tutorial_header.md

# Tutorial 4: Fiber-reinforced matrix

In this tutorial, we benchmark the fiber-reinforced matrix problem proposed in [!cite](bourdin2007numerical).

!media media/matrix_fiber_bcs.png style=display:block;margin:auto;width:60%; caption=Geometry and boundary conditions of the fiber-reinforced matrix problem. id=matrix_fiber_schematics

The boundary conditions are defined as

!listing tutorials/matrix_fiber/elasticity.i
         block=BCs
         language=python

The fiber is assumed to be rigid, hence displacements are fixed on the fiber-matrix interface. The top of the matrix is under displacement control.

For benchmarking purposes, the volumetric-deviatoric decomposition of the strain is required. Such decomposition, along with several other options, are available in [`SmallDeformationIsotropicElasticity`](SmallDeformationIsotropicElasticity.md) and can be specified using the parameter `decomposition`.

!listing tutorials/matrix_fiber/elasticity.i
         block=Materials/elasticity
         language=python

## The complete input files

- [elasticity.i](tutorials/matrix_fiber/elasticity.i)
- [fracture.i](tutorials/matrix_fiber/fracture.i)

!content pagination previous=tutorials/03_mode2_brittle_fracture.md
                    next=tutorials/05_soil_desiccation.md

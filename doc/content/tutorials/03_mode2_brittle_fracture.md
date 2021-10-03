!include tutorials/tutorial_header.md

# Tutorial 3: Mode-II crack propagation

In this tutorial, we will set up the model for Mode-II crack propagation. This problem is almost identical to [Tutorial 2](tutorials/02_mode1_brittle_fracture.md) except for its mesh and boundary conditions.

!media media/mode2_bcs.png style=display:block;margin:auto;width:60%; caption=Geometry and boundary conditions of the Mode-II crack propagation problem. id=mode2_schematics

## Mesh generators

Since we can no longer utilize half symmetry in the Mode-II problem, we will have to mesh the full computational domain. There are many ways of generating the mesh. Here, we demonstrate the use of first-class [`MeshGenerator`](MeshGenerator.md)s to generate the mesh:

!listing tutorials/mode2_brittle_fracture/elasticity.i
         block=Mesh
         language=python

First, the top half of the domain is meshed using the [`GeneratedMeshGenerator`](GeneratedMeshGenerator.md), and a node set named "top_stitch" is identified using the [`BoundingBoxNodeSetGenerator`](BoundingBoxNodeSetGenerator.md). Next, similarly, the bottom half of the domain is meshed and a node set named "bottom_stitch" is identified. Finally, the top half and the bottom half meshes are "stitched" together by merging the "top_stitch" and the "bottom_stitch" node sets using the [`StitchedMeshGenerator`](StitchedMeshGenerator.md). The parameter `construct_side_list_from_node_list` is set to true to construct side sets from all of the node sets.

## `elasticity.i`: The displacement subproblem

The displacement subproblem is almost identical to that in the previous tutorial, except for the boundary conditions [mode2_schematics]:

!listing tutorials/mode2_brittle_fracture/elasticity.i
         block=BCs
         language=python

The bottom boundary of the bottom half of the mesh is fixed in both x- and y- directions, while the top boundary of the top half of the mesh is subject to x-displacement with roller support.

## The complete input files

- [elasticity.i](tutorials/mode2_brittle_fracture/elasticity.i)
- [fracture.i](tutorials/mode2_brittle_fracture/fracture.i)

!content pagination previous=tutorials/02_mode1_brittle_fracture.md
                    next=tutorials/04_fiber_reinforced_matrix.md

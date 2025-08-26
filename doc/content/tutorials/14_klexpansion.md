!include tutorials/tutorial_header.md

# Tutorial 12: Random field generation by KL expansion

In this tutorial, we will generate a spatially varied material property field with a controlled correlation length.

The generated field is ready to be used in [Tutorial 5](tutorials/05_soil_desiccation.md).

## Step 0: required packages

`Boost` required for patch generation. While your Conda MOOSE environment is active, install boost by running:

```
conda install boost
```

## Step 1: build the Karhunen-Loève expansions

For this step, navigate to folder `factorize/`. 

In `klexpansion.C`:

Line 52 provided the mesh for kl expansion construction (this mesh does not have to been identical to the mesh for the physical problem).

```
MeshTools::Generation::build_square(mesh, 50, 50, 0, 100, 0, 100, QUAD4);
```

builds a 2d square mesh occupying coordinate \[0,100\]x\[0,100\], and discretizing by 50x50 QUAD4 elements. This mesh has to be fine enough to fully resolve the correlation length. You need at least 4 nodes per correlation length.\\
Guides on generating other types of meshes [MeshTools:Generation](https://libmesh.github.io/doxygen/namespacelibMesh_1_1MeshTools_1_1Generation.html)\\
Can also read in a mesh by `mesh.read("YourMesh");` 


Line 207-208:

```
  PSE covariance_x(10, 100);
  PSE covariance_y(10, 100);
```

defines the kernel type (PSE or PE), correlation length $L=10$ (or any value given by the user), and periodicity $p=100$ (the size of the domain). Kernels are defined in `covariance_functions.h`

To compile this code

```
make
#  To clean old output and runtime files, run make clean
```

After a successful compilation, run the executable by

```
./klexpansion-opt
```

or in parallel, by

```
mpiexec -n NumberofTasks ./klexpansion-opt 
```

Once it is complete, the KL expansions needed for random field sampling are stored in a new exodus file, by default `basis.e`.

## Step 2: sampling the random field

The step we work in folder `sample/`

In code `sample.C`:

Line 47

```
mesh.read("../factorize/basis.e");
```

reads the KL expansions we just generated in step 1. Update this line as needed, and keep Line 53 reading the same mesh as LIne 47.

Line 78-80:

```
  std::vector<Real> Gc; # the name of field  1
  std::vector<Real> psic; # the name of field  2
  compute_correlated_Gamma_fields(Xi_1, Xi_2, Gc, 8e-4, 0.03, psic, 3e-5, 0.03, 0);
```

Field $G_c$  will be generated with mean $8e-4$ and coefficient of variation $0.03$. Field $\psi_c$ will be generated with mean $3e-5$ and COV $0.03$. The correlation between the two fields is $0$.

To compile the code

```
make
```

To sample a random field

```
./sample-opt
```

The tutorial code generates new field with default name `fields.e`.\\
Each time you run `./sample-opt`, a different field is generated with an update random seed.


## Related source code

- [Factorization](tutorials/klexpansion/factorize/klexpansion.C)
- [Sampling](tutorials/klexpansion/sample/sample.C)


!content pagination previous=tutorials/13_traction_separation.md

!include tutorials/tutorial_header.md

# Tutorial 15: Mosaic field generation (patches)

In this tutorial, we present a simpler approach to generating spatially varied perturbation fields compared to [Tutorial 14](tutorials/14_klexpansion.md).

The subapp `patches.i` first assigns random initial values to the Auxvariable `scale` (raw)

!listing tutorials/patches/patches.i
         block=ICs
         link=False
         language=python

From this raw field, a material field is constructed using selected statistics (e.g., maximum, mean, minimum) within a selected range (e.g., normalized range of [0, 1] )

!listing tutorials/patches/patches.i
         block=Materials
         link=False
         language=python

This coarse field, downsampled by `patchref` steps

!listing tutorials/patches/patches.i
         block=Mesh
         link=False
         language=python

is then transferred to the fine mesh of the mainapp `main.i`

!listing tutorials/patches/main.i
         block=Transfers
         link=False
         language=python

The resulting fields are illustrated in the figure below.

!media media/main_sigma_ts.png style=width:32%;float:left; caption=Fig. 1: Mainapp mosaic field $\sigma_{ts}$.

!media media/sub_sigma_ts.png style=width:32%;float:left;margin-left:2%; caption=Fig. 2: Subapp field $\sigma_{ts}$.

!media media/sub_scale.png style=width:32%;float:right; caption=Fig. 3: Subapp scale variable.

## The complete input files

- [main.i](tutorials/patches/main.i)
- [patches.i](tutorials/patches/patches.i)

!content pagination previous=tutorials/14_klexpansion.md



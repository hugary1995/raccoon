# ComputeLargeDeformationStress

!syntax description /Materials/ComputeLargeDeformationStress

## Overview

The input of this stress calculator are the mechanical deformation gradient $\bfF^m$ and optionally its old value $\bfF^m_{n-1}$. An elasticity model must be provided. A plasticity model and a viscoelasticity model can be optionally provided.

The stress calculator calls the elasticity model to compute the Cauchy stress given the mechanical deformation gradient.

If a plasticity model is provided, plastic deformation will be updated to bring the stress state back onto the yield surface.

If a viscoelasticity model is provided, the stress calculator will call the viscoelasticity model to compute the viscous stress. The viscous stress will be added to the equilibrium stress returned by the elasticity model.

## Example Input File Syntax

!syntax parameters /Materials/ComputeLargeDeformationStress

!syntax inputs /Materials/ComputeLargeDeformationStress

!syntax children /Materials/ComputeLargeDeformationStress

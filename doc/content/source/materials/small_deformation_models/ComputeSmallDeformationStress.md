# ComputeSmallDeformationStress

!syntax description /Materials/ComputeSmallDeformationStress

## Overview

The input of this stress calculator is the mechanical strain $\strain^m$. An elasticity model must be provided. A plasticity model can be optionally provided.

The stress calculator calls the elasticity model to compute the Cauchy stress given the mechanical strain.

If a plasticity model is provided, plastic deformation will be updated to bring the stress state back onto the yield surface.

## Example Input File Syntax

!syntax parameters /Materials/ComputeSmallDeformationStress

!syntax inputs /Materials/ComputeSmallDeformationStress

!syntax children /Materials/ComputeSmallDeformationStress

# SmallDeformationIsotropicElasticity

!syntax description /Materials/SmallDeformationIsotropicElasticity

## Overview

The strain energy density is defined as

\begin{equation}
  \psi^e = \dfrac{1}{2} K \tr(\strain)^2 + G \dev(\strain):\dev(\strain).
\end{equation}

This class features three most commonly used splits of the strain energy, i.e. no decomposition, spectral decomposition, and volumetric-deviatoric decomposition.

## Example Input File Syntax

!syntax parameters /Materials/SmallDeformationIsotropicElasticity

!syntax inputs /Materials/SmallDeformationIsotropicElasticity

!syntax children /Materials/SmallDeformationIsotropicElasticity

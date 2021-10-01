# LargeDeformationNewtonianViscosity

## Overview

Consider the elastic viscous dissipation defined as

\begin{equation}
  R = \dfrac{1}{2} g J \eta \norm{\dev(\bfD)}^2, \quad \bfD = \sym\left( \dot{\bfF}^m {\bfF^m}^{-1} \right),
\end{equation}

where $g$ is the degradation function, $J$ is the jacobian of deformation gradient, $\eta$ is the viscosity, $\bfF^m$ is the so-called mechanical deformation gradient after excluding the effect of eigen deformation gradients. Following variational principles, the Newtonian dissipative stress is given as

\begin{equation}
  \stress^\text{dis} = g \eta \dev{\bfD},
\end{equation}

defined in the current configuration.

## Example Input File Syntax

!! Describe and include an example of how to use the LargeDeformationNewtonianViscosity object.

!syntax parameters /Materials/LargeDeformationNewtonianViscosity

!syntax inputs /Materials/LargeDeformationNewtonianViscosity

!syntax children /Materials/LargeDeformationNewtonianViscosity

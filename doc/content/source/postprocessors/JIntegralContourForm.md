# JIntegralContourForm

!syntax description /UserObjects/JIntegralContourForm

## Overview

The contour form J-integral is defined as

\begin{equation}
  J = \int\limits_{\bodyboundary} \bft \cdot \bs{\Sigma} \bfn \diff{s},
\begin{equation}

where $\bft$ is the unit vector in the direction of crack growth, $\bfn$ is the surface normal, and $\bs{\Sigma}$ is the energy-momentum tensor defined as

\begin{equation}
  \bs{\Sigma} = \psi^e \bfI - \bfH^T \stress,
\end{equation}

where $\psi^e$ is the elastic energy, $\bfI$ is the identity tensor, $\bfH$ is the gradients of displacements.

## Example Input File Syntax

!syntax parameters /UserObjects/JIntegralContourForm

!syntax inputs /UserObjects/JIntegralContourForm

!syntax children /UserObjects/JIntegralContourForm

# LargeDeformationJIntegral

!syntax description /UserObjects/LargeDeformationJIntegral

## Overview

The J-integral is defined as

\begin{equation}
  J = \int\limits_{\bodyboundary_0} \bft \cdot \bs{\Sigma} \bfn_0 \diff{s},
\begin{equation}

where $\bft$ is the unit vector in the direction of crack growth, $\bfn_0$ is the surface normal in the reference configuration, and $\bs{\Sigma}$ is the energy-momentum tensor defined as

\begin{equation}
  \bs{\Sigma} = \psi^e \bfI - \bfH^T \bfP, \quad \bfH = \bfF - \bfI,
\end{equation}

where $\psi^e$ is the elastic energy, $\bfI$ is the identity tensor, $\bfF$ is the deformation gradient (excluding eigen deformation gradients), and $\bfP$ is the first Piola-Kirchhoff stress.

## Example Input File Syntax

!syntax parameters /UserObjects/LargeDeformationJIntegral

!syntax inputs /UserObjects/LargeDeformationJIntegral

!syntax children /UserObjects/LargeDeformationJIntegral

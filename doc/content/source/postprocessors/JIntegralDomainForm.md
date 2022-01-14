# JIntegralDomainForm

!syntax description /UserObjects/JIntegralDomainForm

## Overview

The domain form J-integral is defined as

\begin{equation}
  J = - \int\limits_{\body} \bft \cdot \bs{\Sigma} \grad q \diff{V},
\begin{equation}

where $\bft$ is the unit vector in the direction of crack growth, $q$ is the domain indicator function ($q = 1$ inside the domain where the J-integral is being evaluated, and $q = 0$ elsewhere), and $\bs{\Sigma}$ is the energy-momentum tensor defined as

\begin{equation}
  \bs{\Sigma} = \psi^e \bfI - \bfH^T \stress,
\end{equation}

where $\psi^e$ is the elastic energy, $\bfI$ is the identity tensor, $\bfH$ is the gradients of displacements.

## Example Input File Syntax

!syntax parameters /UserObjects/JIntegralDomainForm

!syntax inputs /UserObjects/JIntegralDomainForm

!syntax children /UserObjects/JIntegralDomainForm

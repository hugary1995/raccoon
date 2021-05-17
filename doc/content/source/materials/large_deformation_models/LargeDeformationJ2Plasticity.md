# LargeDeformationJ2Plasticity

!syntax description /Materials/LargeDeformationJ2Plasticity

## Overview

Following [the theory](theory/minimization.md), Given the Prandtl-Reuss flow rule,

\begin{equation}
  \begin{aligned}
    \tr\left( \dot{\bfF}^p {\bfF^p}^{-1} \right) &= 0, \\
    \norm{\dot{\bfF}^p {\bfF^p}^{-1}}^2 - \dfrac{3}{2} \abs{\epdot}^2 &= 0,
  \end{aligned}
\end{equation}

minimizing the total potential jointly with respect to $\epdot$ and $\dot{\bfF}^p$ (say, using Lagrange multipliers) gives the yield surface:

\begin{equation}
  \phi^p = \norm{\dev(\bfM)} - \sqrt{\dfrac{2}{3}} \psi^p_{, \epdot},
\end{equation}

where $\bfM$ is the Mandel stress and $\psi^p$ is the plastic energy.

This class uses the exponential update to integrate the constitutive constraint in time. The plasticity model calls the elasticity model to get the value of the Mandel stress, and it calls the plastic hardening model to get the value of the plastic energy and its derivatives with respect to $\epdot$.

## Example Input File Syntax

!syntax parameters /Materials/LargeDeformationJ2Plasticity

!syntax inputs /Materials/LargeDeformationJ2Plasticity

!syntax children /Materials/LargeDeformationJ2Plasticity

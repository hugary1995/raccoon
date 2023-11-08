# ADComputePorosity

!syntax description /Materials/ADComputePorosity

## Overview

This equation calculates porosity evolution in porous solids in terms of variation of fluid pressure and solid volumetric strain:

\begin{equation}
  \phi = \phi_0 + \frac{p}{M} + \alpha \epsilon_v,
\end{equation}
where $\phi$ is the solid porosity; $\phi_0$ is the initial solid porosity; $M$ is biot's modulus; $\alpha$ is the Biot’s coefficient; and $\epsilon_v$ is the solid volumetric strain.

## Example Input File Syntax

!syntax parameters /Materials/ADComputePorosity

!syntax inputs /Materials/ADComputePorosity

!syntax children /Materials/ADComputePorosity

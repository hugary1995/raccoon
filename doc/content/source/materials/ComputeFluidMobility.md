# ComputeFluidMobility

!syntax description /Materials/ComputeFluidMobility

## Overview

\begin{equation}
  \boldsymbol{\lambda} = \frac{\bold{k}}{\mu} + d^{\xi}\frac{\bold{k}^d}{\mu},
\end{equation}
where
in which $\xi$ is an exponent used to localize the increased permeability along the fracture. $\bold{k}/\mu$ is the fluid mobility in intact bulk porous solid, while $\bold{k}^d/\mu$ is the additional permeability in the damaged domain.

## Example Input File Syntax

!syntax parameters /Materials/ComputeFluidMobility

!syntax inputs /Materials/ComputeFluidMobility

!syntax children /Materials/ComputeFluidMobility

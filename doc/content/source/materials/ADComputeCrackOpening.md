# ADComputeCrackOpening

!syntax description /Materials/ADComputeCrackOpening

## Overview

Crack opening calculation is accomplished using the gradient of damage variable, $\boldsymbol{\nabla}d$, which is used to define the unit vector perpendicular to the crack plane as $\bold{n}^d = \boldsymbol{\nabla}d/|\boldsymbol{\nabla}d|$. The crack opening field is then calculated as

\begin{equation}
\varw_n = h^{el} \left(\bold{n}^d \cdot \boldsymbol{\epsilon} \cdot \bold{n}^d\right), 
\end{equation}
where $h^{el}$ is the edge length of the current element.

## Example Input File Syntax

!syntax parameters /Materials/ADComputeCrackOpening

!syntax inputs /Materials/ADComputeCrackOpening

!syntax children /Materials/ADComputeCrackOpening

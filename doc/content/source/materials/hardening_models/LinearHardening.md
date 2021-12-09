# LinearHardening

!syntax description /Materials/LinearHardening

## Overview

This plastic hardening model defines the following plastic energy (and its derivatives)

\begin{equation}
\begin{aligned}
\psi^p = g \left( \sigma_y \ep + \dfrac{1}{2} H \ep^2 \right), \\
\end{aligned}
\end{equation}
where $\sigma_y$ is the yield stress and $H$ is the hardening modulus.

## Example Input File Syntax

!syntax parameters /Materials/LinearHardening

!syntax inputs /Materials/LinearHardening

!syntax children /Materials/LinearHardening

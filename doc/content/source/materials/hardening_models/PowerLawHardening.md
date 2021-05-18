# PowerLawHardening

!syntax description /Materials/PowerLawHardening

## Overview

This plastic hardening model defines the following plastic energy (and its derivatives)

\begin{equation}
  \begin{aligned}
    \psi^p = \dfrac{n}{n+1}g \sigma_y \epsilon_0 \left[ \left( 1+\dfrac{\ep}{\epsilon_0} \right)^{\dfrac{n+1}{n}} - 1 \right], \\
  \end{aligned}
\end{equation}

## Example Input File Syntax

!syntax parameters /Materials/PowerLawHardening

!syntax inputs /Materials/PowerLawHardening

!syntax children /Materials/PowerLawHardening

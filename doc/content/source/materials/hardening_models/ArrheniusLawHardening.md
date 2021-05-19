# ArrheniusLawHardening

!syntax description /Materials/ArrheniusLawHardening

## Overview

This plastic hardening law follows from the following plastic energy:

\begin{equation}
  \psi^p = g \sigma_0 / A + 0.5 \epsilon \ep^2,
\end{equation}
where $g$ is the degradation function, $\sigma_0$ is the reference stress, and $A$ is a coefficient following the Arrhenius law. A linear hardening term, i.e. $0.5 \epsilon \ep^2$ is added to optional add linear hardening.

## Example Input File Syntax

!syntax parameters /Materials/ArrheniusLawHardening

!syntax inputs /Materials/ArrheniusLawHardening

!syntax children /Materials/ArrheniusLawHardening

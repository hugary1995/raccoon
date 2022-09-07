# JohnsonCookHardening

!syntax description /Materials/JohnsonCookHardening

## Overview

This plastic hardening model defines the following plastic energy (and its derivatives)

\begin{equation}
\psi_p = \left(1-Q\right)g_p \sigma_0 \ep \left(A\ep + B\ep0\left(\frac{\ep}{\ep0}\right)^{n+1}\frac{1}{n+1}\right),\\
\psi_p^* = Q \sigma_0
\end{equation}

where $\sigma_\infty$ is the ultimate tensile stress and $\varepsilon_0$ is the reference plastic strain.

The corresponding yield surface is given as

\begin{equation}
g_p \left[ \sigma_\infty - ( \sigma_\infty - \sigma_y) \exp \left( -\dfrac{\ep}{\varepsilon_0} \right) + H \ep \right].
\end{equation}

## Example Input File Syntax

!syntax parameters /Materials/JohnsonCookHardening

!syntax inputs /Materials/JohnsonCookHardening

!syntax children /Materials/JohnsonCookHardening

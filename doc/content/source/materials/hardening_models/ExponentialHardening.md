# ExponentialHardening

!syntax description /Materials/ExponentialHardening

## Overview

This plastic hardening model defines the following plastic energy (and its derivatives)

\begin{equation}
\psi_p = g_p \sigma_\infty \ep + g_p \varepsilon_0 \left[ \sigma_\infty - \sigma_y \exp \left( -\dfrac{\ep}{\varepsilon_0} \right) - 1 \right] + \dfrac{1}{2} g_p H \ep^2,
\end{equation}

where $\sigma_\infty$ is the ultimate tensile stress and $\varepsilon_0$ is the reference plastic strain.

The corresponding yield surface is given as

\begin{equation}
g_p \left[ \sigma_\infty - ( \sigma_\infty - \sigma_y) \exp \left( -\dfrac{\ep}{\varepsilon_0} \right) + H \ep \right].
\end{equation}

## Example Input File Syntax

!syntax parameters /Materials/ExponentialHardening

!syntax inputs /Materials/ExponentialHardening

!syntax children /Materials/ExponentialHardening

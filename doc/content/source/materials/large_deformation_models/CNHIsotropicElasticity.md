# CNHIsotropicElasticity

!syntax description /Materials/CNHIsotropicElasticity

## Overview

The compressible Neo-Hookean material has the strain energy defined as

\begin{equation}
  \begin{aligned}
    \psi^e &= g \psi^e_\activepart + \psi^e_\inactivepart, \\
    \psi^e_\activepart &= \mathbb{H}_1(J)\left\{ \dfrac{1}{2}K\left[ \dfrac{1}{2}(J^2-1) - \ln{J} \right] \right\} + \dfrac{1}{2}G\left( \bar{\bfC}:{\bfC^p}^{-1} - 3 \right), \\
    \psi^e_\inactivepart &= \left( 1-\mathbb{H}_1(J) \right) \left\{ \dfrac{1}{2}K\left[ \dfrac{1}{2}(J^2-1) - \ln{J} \right] \right\},
  \end{aligned}
\end{equation}

where $\mathbb{H}_x$ is the Heaviside function centered at $x$ and $g$ is the degradation function. The Kirchhoff stress follows as

\begin{equation}
  \begin{aligned}
    \bs{\tau} &= g\bs{\tau}_\activepart + \bs{\tau}_\inactivepart, \\
    \bs{\tau}_\activepart &= \dfrac{1}{2}\mathbb{H}_1(J)K(J^2-1)\bfI + G\dev(\bfb^e), \\
    \bs{\tau}_\inactivepart &= \dfrac{1}{2}(1-\mathbb{H}_1(J))K(J^2-1)\bfI,
  \end{aligned}
\end{equation}

where $\bfb^e$ is the elastic left Cauchy-Green strain.

## Example Input File Syntax

!syntax parameters /Materials/CNHIsotropicElasticity

!syntax inputs /Materials/CNHIsotropicElasticity

!syntax children /Materials/CNHIsotropicElasticity

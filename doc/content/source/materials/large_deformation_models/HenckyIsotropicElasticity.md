# HenckyIsotropicElasticity

!syntax description /Materials/HenckyIsotropicElasticity

## Overview

The Hencky-type hyperelastic material assumes the following strain energy density:

\begin{equation}
  \psi^e = \dfrac{1}{2} K \tr(\bs{\varepsilon}^e)^2 + G \dev(\bs{\varepsilon}^e) : \dev(\bs{\varepsilon}^e),
\end{equation}

where $\bs{\varepsilon}^e$ is a logarithmic strain measure based on the right Cauchy-green strain defined as

\begin{equation}
  \bs{\varepsilon}^e = \dfrac{1}{2} \log (\bfC^e).
\end{equation}

The stress conjugate follows from the variational principles.

\begin{equation}
  \bs{\Sigma} = K \tr(\bs{\varepsilon}^e) \bfI + 2 G \dev(\bs{\varepsilon}^e).
\end{equation}

## Example Input File Syntax

!syntax parameters /Materials/HenckyIsotropicElasticity

!syntax inputs /Materials/HenckyIsotropicElasticity

!syntax children /Materials/HenckyIsotropicElasticity

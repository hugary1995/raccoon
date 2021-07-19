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

Three types of splits are available

1. No Split

\begin{equation}
    w_{\langle A\rangle}^e(\bfC^e) = \frac{1}{2}K \tr(\bs{\varepsilon})^2 + G \text{ dev } \bs{\varepsilon}^e:\text{dev } \bs{\varepsilon}^e
\end{equation}

2. Volumetric Deviatoric Split

\begin{equation}
  w_{\langle A\rangle}^e(\bfC^e) = \frac{1}{2}K\langle \tr(\bs{\varepsilon}^e)\rangle_{+}+G \text{ dev} \bs{\varepsilon}^e:\text{dev} \bs{\varepsilon}^e
\end{equation}
\begin{equation}
  w_{\langle I \rangle}^e(\bfC^e) = \frac{1}{2}K\langle \tr(\bs{\varepsilon}^e)\rangle_-
\end{equation}


3. Spectral Decomposition

\begin{equation}
  w_{\langle A \rangle}^e(\bfC^e) = \frac{1}{2}\lambda \langle \bs{\varepsilon}_1^e + \bs{\varepsilon}_2^e + \bs{\varepsilon}_3^e\rangle_+^2 + G(\langle \bs{\varepsilon}_1\rangle_+^2+\langle\bs{\varepsilon}_2\rangle_+^2+\langle\bs{\varepsilon}_3\rangle_+^2)
\end{equation}
\begin{equation}
    w_{\langle I \rangle}^e(\bfC^e) = \frac{1}{2}\lambda \langle \bs{\varepsilon}_1^e + \bs{\varepsilon}_2^e + \bs{\varepsilon}_3^e\rangle_-^2 + G(\langle \bs{\varepsilon}_1\rangle_-^2+\langle\bs{\varepsilon}_2\rangle_-^2+\langle\bs{\varepsilon}_3\rangle_-^2)
\end{equation}


## Example Input File Syntax

!syntax parameters /Materials/HenckyIsotropicElasticity

!syntax inputs /Materials/HenckyIsotropicElasticity

!syntax children /Materials/HenckyIsotropicElasticity

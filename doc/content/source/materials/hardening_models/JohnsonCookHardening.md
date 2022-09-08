# JohnsonCookHardening

!syntax description /Materials/JohnsonCookHardening

## Overview

The Johnson-Cook plasticity model has the following form
\begin{equation}
    \sigma = \sigma_0\left(A+B\left(\frac{\ep}{\ep0}\right)^n\right)\left(1+C\text{ln}\left(\frac{\epdot}{\epdot0}\right)\right)\left(1-\left(\frac{T-T_0}{T_m-T_0}\right)^m\right)
\end{equation}

Where $\sigma_0$ is the unperturbed yield stress, $\ep0$ is the reference plastic strain, $\epdot0$ is the reference plastic strain rate, $T_m$ is the melting temperature, and $T_0$ is the reference temperature.
$A$,$B$,and $C$ are parameters of the Johnson-Cook model.

When put into a variational framework, the Johnson-Cook model defines the following plastic energy
\begin{equation}
\psi_p = \left[\left(1-Q\right)g_p \sigma_0 \ep \left(A\ep + B\ep0\left(\frac{\ep}{\ep0}\right)^{n+1}\frac{1}{n+1}\right)\right]f(T),\\
\psi_p^* = \left[Q \sigma_0\left(A+B\ep0\left(\frac{\ep}{\ep0}\right)^n\right)+\left(A+B\frac{\ep}{\ep0}\right)\left(C\text{ln}\left(\frac{\epdot}{\epdot0}\right)-C\right)\right]f(T),\\
f(T) = \left(\frac{T-T_0}{T_m-T_00}\right)
\end{equation}

Additionally, the energy is split into energetic and dissipative portions using the Taylor-Quinney factor, $Q$.\\
The flow stresses are defined as the following

\begin{equation}
    Y^{\text{eq}}=\left[\left(1-Q\right)\sigma_0\left(A+B\frac{\ep}{\ep0}\right)^n\right]f(T),\\
    Y^{\text{vis}}=\left[Q\sigma_0\left(A+B\left(\frac{\ep}{\ep0}\right)^n\right)+\left(A+B\frac{\ep}{\ep0}\right)\left(C\text{ln}\left(\frac{\epdot}{\epdot0}\right)\right)\right]f(T)
\end{equation}


The corresponding yield surface is given as

\begin{equation}
    \phi_p=||\text{dev}\left(M\right)||-\sqrt{\frac{3}{2}}\left(Y^{\text{eq}}+Y^{\text{vis}}\right)
\end{equation}

Where $M$ is the Mandel stress.


!syntax parameters /Materials/JohnsonCookHardening

!syntax inputs /Materials/JohnsonCookHardening

!syntax children /Materials/JohnsonCookHardening

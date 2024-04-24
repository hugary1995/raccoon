# KLRNucleationMicroForce

!syntax description /Materials/KLRNucleationMicroForce

In general, model KLR published in 2022 [!cite](Kumar2022) is recommended over model KLBF published in 2020 [!cite](KUMAR2020104027) for better performance in compression. It also allows the use of smaller $\delta$ values that reduce the mesh burden.

## Overview 

### KLR (Kumar, Lopez, Ravi-Chandar) Model 2022

By adding an extra term in the governing equation for fracture, $c_e$, the material's strength surface can be effectively employed as the damage initiation criterion,

\begin{equation}
g'(d) W + \frac{G_c}{c_0 \ell_0} ( \alpha' (d) - 2 \ell_0 \Delta d)  + c_e \geq 0.
\end{equation}

The external driving force is in the form of:

\begin{equation}
    c_e(I_1,J_2;l_0)=\beta^{l}_2\sqrt{J_2}+\beta^{l}_1I_1+\beta^{l}_0+(1-\frac{\sqrt{I_1^2}}{I_1})(\frac{J_2}{2\mu}+\frac{I_1^2}{6(3\lambda+2\mu)})
\end{equation}

with material properties and simulation parameter dependent constants defined as

\begin{equation}
\beta_0^{l_0}
=\delta\frac{3G_c}{8l_0},
\end{equation}

\begin{equation}
\beta_1^{l_0}
=-\frac{(1+\delta)(\sigma_{cs}-\sigma_{ts})}{2\sigma_{cs}\sigma_{ts}}\frac{3G_c}{8l}
+\frac{\sigma_{ts}}{6(3\lambda+2\mu)}+\frac{\sigma_{ts}}{6\mu},
\end{equation}

\begin{equation}
\beta_2^{l_0}
=-\frac{\sqrt{3}(1+\delta)(\sigma_{cs}+\sigma_{ts})}{2\sigma_{cs}\sigma_{ts}}\frac{3G_c}{8l_0}
+\frac{\sqrt{3}\sigma_{ts}}{6(3\lambda+2\mu)}+\frac{\sqrt{3}\sigma_{ts}}{6\mu}.
\end{equation}

$I_1$ and $J_2$ are invariants of the Cauchy stress:

\begin{equation}
    I_1=tr(\boldsymbol{\sigma}), \quad
\end{equation}

\begin{equation}
    J_2=\frac{1}{2}(\boldsymbol{\sigma}-\frac{1}{3}(tr(\boldsymbol{\sigma})\boldsymbol{I})):(\boldsymbol{\sigma}-\frac{1}{3}(tr(\boldsymbol{\sigma})\boldsymbol{I})).
\end{equation}

The numerical strength surface introduced by the external driving force is

\begin{equation}
\hat{F}^{l_0}(I_1,J_2)=\frac{J_2}{\mu}+\frac{I_1^2}{9\kappa}-c_e(I_1,J_2;l_0)-\frac{3G_c}{8l_0}=0,
\end{equation}

defined in terms of the critical tensile strength $\sigma_{ts}$ and the critical compressive strength $\sigma_{cs}$. Other locations on the surface approach to the Drucker-Prager criterion from [!cite](Drucker-Prager) as the phase-field regularization length approaches zero:

\begin{equation}
    \hat{F}(I_1,J_2)
    =\sqrt{J_2}
    +\frac{\sigma_{cs}-\sigma_{ts}}{\sqrt{3}(\sigma_{cs}+\sigma_{ts})}I_1
    -\frac{2\sigma_{cs}\sigma_{ts}}{\sqrt{3}(\sigma_{cs}+\sigma_{ts})}I_1=0
\end{equation}

$l_0$ is the prescribed regularization length. By enforcing the irreversibility restriction only when the material is close to fully damaged, say $d\ge 0.95$, the $G_c$ behavior of Griffith model can be kept by calibrating parameter $\delta$. Currently we use a [surfing boundary problem](tutorials/12_surfing_boundary_problem.md) to match the precribed $G_c$ and the J intergral. Be aware that this $\delta$ is currently dependent on material properties and model parameters (regularization length, mesh size,...). In other words, calibration of $\delta$ is necessary.

During crack propagation, the effective regularization length $l_e$ 

\begin{equation}
l_e = \frac{l_0}{\sqrt{1+\delta}},\quad (\delta > -1)
\end{equation}

could deviate from $\ell$. The mesh should be able to resolve $l_e$ as well.

### KLBF (Kumar, Lopez, Bourdin, Francfort) Model 2020

See [KLBFNucleationMicroForce](nucleation_models/KLBFNucleationMicroForce.md)

## Example Input File Syntax

!syntax parameters /Materials/KLRNucleationMicroForce

!syntax inputs /Materials/KLRNucleationMicroForce

!syntax children /Materials/KLRNucleationMicroForce

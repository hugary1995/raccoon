# NucleationMicroForce

!syntax description /Materials/NucleationMicroForce

In general, model published in 2022 [!cite](https://doi.org/10.1007/s10704-022-00653-z) is recommended over model 2020[!cite](https://doi.org/10.1016/j.jmps.2020.104027) for better performance in compression. It also allows the use of smaller $\delta$ values that reduce the mesh burden.

## Overview 

### Model 2022

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

defined in terms of the critical tensile strength $\sigma_{ts}$ and the critical compressive strength $\sigma_{cs}$. Other locations on the surface approach to the Drucker-Prager criterion [!cite](https://www.jstor.org/stable/43633942) as the phase-field regularization length approaches zero:

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

Here is an example input deck

!listing /tutorials/surfing_boundary_problem/fracture.i block=Materials/kumar_material

### Model 2020

The two versions follow the same idea with differnt construction of the model parameters.

The external driving force of model published in 2020 is in the form of:

\begin{equation}
    c_e(I_1,J_2;l_0)=\frac{1}{1+\beta_3^{l_0}I_1^2}( \beta^{l_0}_2\sqrt{J_2}+\beta^{l_0}_1I_1+\beta^{l_0}_0),
\end{equation} 

with material properties and simulation parameter dependent constants defined as

\begin{equation}
\beta_0^{l_0}=\delta\frac{3G_c}{8l_0}
\end{equation}

\begin{equation}
\beta_1^{l_0}
=-\frac{(1+\delta)(\sigma_{cs}-\sigma_{ts})}{2\sigma_{cs}\sigma_{ts}}\frac{3G_c}{8l_0}
-\frac{(8\mu + 24\kappa - 27\sigma_{ts})(\sigma_{cs}-\sigma_{ts})}{144\mu\kappa}
-\frac{(\mu+3\kappa)(\sigma_{cs}^3-\sigma_{ts}^3)\sigma_{ts}}{18\mu^2\kappa^2}\frac{l_0}{G_c},
\end{equation}

\begin{equation}
\beta_2^{l_0}
=-\frac{\sqrt{3}(1+\delta)(\sigma_{cs}+\sigma_{ts})}{2\sigma_{cs}\sigma_{ts}}\frac{3G_c}{8l_0}
+\frac{(8\mu + 24\kappa - 27\sigma_{ts})(\sigma_{cs}+\sigma_{ts})}{48\sqrt{3}\mu\kappa}
+\frac{(\mu+3\kappa)(\sigma_{cs}^3-\sigma_{ts}^3)\sigma_{ts}}{6\sqrt{3}\mu^2\kappa^2}\frac{l_0}{G_c},
\end{equation}

\begin{equation}
\beta_3^{l_0}=\frac{l_0\sigma_{ts}}{\mu\kappa G_c}.
\end{equation}

## Example Input File Syntax

!syntax parameters /Materials/NucleationMicroForce

!syntax inputs /Materials/NucleationMicroForce

!syntax children /Materials/NucleationMicroForce

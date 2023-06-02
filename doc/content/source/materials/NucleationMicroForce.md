# NucleationMicroForce

!syntax description /Materials/NucleationMicroForce

In general, model published in 2022 [!cite](https://doi.org/10.1007/s10704-022-00653-z) is recommended over model 2020[!cite](https://doi.org/10.1016/j.jmps.2020.104027)for better performance in compression and smaller $\delta$ value that reduce the mesh burden.

## Overview 

### Model 2022

By adding an extra term in the governing equation for fracture, $c_e$, the material strength surface is employed as damage initiation criteria,

\begin{equation}
g'(d) W + \frac{G_c}{c_0 \ell_0} ( \alpha' (d) - 2 \ell_0 \Delta d)  + c_e \ge 0.
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

The $I_1$ and $J_2$ are invariants for the degraded Cauchy stress:

\begin{equation}
    I_1=tr(\boldsymbol{\sigma}), \quad
\end{equation}

\begin{equation}
    J_2=\frac{1}{2}(\boldsymbol{\sigma}-\frac{1}{3}(tr(\boldsymbol{\sigma})\boldsymbol{I})):(\boldsymbol{\sigma}-\frac{1}{3}(tr(\boldsymbol{\sigma})\boldsymbol{I})).
\end{equation}

The numerical stress envelope introduced by the external driving force is

\begin{equation}
\hat{F}^{l_0}(I_1,J_2)=\frac{J_2}{\mu}+\frac{I_1^2}{9\kappa}-c_e(I_1,J_2;l_0)-\frac{3G_c}{8l_0}=0,
\end{equation}

which strictly satisfied the critical tensile strength $\sigma_{ts}$ and critical compressive strength $\sigma_{cs}$. Other locations on the surface approach to an existed model for the stress envelope, the Drucker-Prager model[!cite](https://www.jstor.org/stable/43633942), as regularization length approach zero limit:

\begin{equation}
    \hat{F}(I_1,J_2)
    =\sqrt{J_2}
    +\frac{\sigma_{cs}-\sigma_{ts}}{\sqrt{3}(\sigma_{cs}+\sigma_{ts})}I_1
    -\frac{2\sigma_{cs}\sigma_{ts}}{\sqrt{3}(\sigma_{cs}+\sigma_{ts})}I_1=0
\end{equation}

$l_0$ is the prescribed regularization length. By enforcing the irreversibility restriction only when the material is close to fully damaged, say $d\ge 0.95$, the $G_c$ behavior of Griffith model can be kept by calibrating parameter $\delta$. Currently we use a [surfing boundary problem](tutorials/12_surfing_boundary_problem.md) to match the precribed $G_c$ and the J intergral. Be aware that this $\delta$ is currently dependent on material properties and model parameters (regularization length, mesh size,...) until further developed version. Do calibrate it when any of the parameter is changed.

During crack propagation, the effective regularization length $l_e$ 

\begin{equation}
l_e = \frac{l_0}{\sqrt{1+\delta}},\quad (\delta > -1)
\end{equation}

could be different than the prescribed one. Make sure your mesh size can also fully resolve it.

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

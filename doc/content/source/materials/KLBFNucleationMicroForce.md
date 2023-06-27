# KLBFNucleationMicroForce

!syntax description /Materials/KLBFNucleationMicroForce

In general, model KLR published in [!cite](Kumar2022) is recommended over model KLBF published in [!cite](KUMAR2020104027) for better performance in compression. It also allows the use of smaller $\delta$ values that reduce the mesh burden.

## Overview 

### KLR (Kumar, Lopez, Ravi-Chandar) Model 2022

Please see [KLRNucleationMicroForce](source/materials/KLRNucleationMicroForce.md)

### KLBF (Kumar, Lopez, Bourdin, Francfort) Model 2020

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

!syntax parameters /Materials/KLBFNucleationMicroForce

!syntax inputs /Materials/KLBFNucleationMicroForce

!syntax children /Materials/KLBFNucleationMicroForce

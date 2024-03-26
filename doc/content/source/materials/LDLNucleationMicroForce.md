# LDLNucleationMicroForce

!syntax description /Materials/LDLNucleationMicroForce

See *A variational formulation of Griffith phase-field fracture with material strength* by [!cite](larsen2024)

This is a variational recast of the phase-field fracture formulation put forth by Kumar, Francfort and Lopez-Pamies (2018), where $\delta$ is explicitly evaluated.

## Overview

### LDL (Larsen, Dolbow, Lopez) Model 2024

The fracture functional reads

\begin{equation}
\mathcal{E}_f^l(\boldsymbol{u}, d):= \int_\Omega g(d) \psi_e(\boldsymbol{\varepsilon}(\boldsymbol{u})) \;\mathrm{dV} + \frac{\delta^l G_c}{c_0}\int_\Omega\left(\frac{\alpha(d)}{l} + l\nabla d\cdot \nabla d\right) \;\mathrm{dV} - \int_\Omega\left(\int_0^1 g(d) \;\mathrm{dd}\right)\widehat{c_e}(\boldsymbol{\varepsilon}(\boldsymbol{u})) \;\mathrm{dV}.
\end{equation}

The governing equation for fracture is
\begin{equation}
-\nabla\cdot \dfrac{2 G_c l \delta^l}{c_0}\nabla d + \dfrac{\partial}{\partial d}\left( \alpha \dfrac{\delta^l G_c}{c_0 l} - g(d)\psi_e\right) - g(d)\widehat{c_e} \ge 0.
\end{equation}

Note that the following derivation follows settings of `AT-1`, i.e., $\alpha(d)=d$ and $c_0=\dfrac{8}{3}$. In this model, the strict irreversibility condition for $d$ is adopted.

The "undamaged" nucleation driving force is defined as
\begin{equation}
\widehat{c_e}(\boldsymbol{\varepsilon}(\boldsymbol{u}))= \alpha_2 \sqrt{J_2} +  \alpha_1 I_1 + (1-d)\left(1 - \dfrac{\sqrt{I_1^2}}{I_1}\right)\psi_e.
\end{equation}

In this recast, $\delta^l$ is evaluated explicitly as
\begin{equation}
\text{without h correction:}\quad\delta^l=\left(\frac{\sigma_{ts}+ (1+2\sqrt{3})\sigma_{hs}}{(8+3\sqrt{3})\sigma_{hs}}\right)\frac{3G_c}{16 \psi_{ts} l} + \frac{3}{8},
\end{equation}

\begin{equation}
\text{with h correction:}\quad\delta^l=\left(1+\frac{3 h}{8 l}\right)^{-2}\left(\frac{\sigma_{ts}+(1+2\sqrt{3})\sigma_{hs}}{(8+3\sqrt{3})\sigma_{hs}}\right)\frac{3G_c}{16 \psi_{ts} l} + \left(1 + \frac{3h}{8l}\right)^{-1}\frac{2}{5},
\end{equation}

where
\begin{equation}
\alpha_1 = - \frac{1}{\sigma_{hs}}\delta^l \frac{G_c}{8l} + \frac{2\psi_{hs}}{3\sigma_{hs}}, \qquad \alpha_2= - \frac{\sqrt{3}(3\sigma_{hs} - \sigma_{ts})}{\sigma_{hs}\sigma_{ts}}\delta^l \frac{G_c}{8l} - \frac{2\psi_{hs}}{\sqrt{3}\sigma_{hs}} + \frac{2\sqrt{3}\psi_{ts}}{\sigma_{ts}},
\end{equation}

with
\begin{equation}
\psi_{ts}=\frac{\sigma^2_{ts}}{2E}, \quad \psi_{hs}=\frac{\sigma^2_{hs}}{2\kappa}.
\end{equation}

The material properties used are the bulk modulus $\kappa$ and Young's modulus $E$ and the shear modulus $\mu$.

The strength surface is presented with uniaxial tensile $\sigma_{ts}$ and hydrostatic strength $\sigma_{hs}$, where $\sigma_{hs}=\dfrac{2}{3} \dfrac{\sigma_{ts}\sigma_{cs}}{\sigma_{cs} - \sigma_{ts}}$.

### KLR (Kumar, Lopez, Ravi-Chandar) Model 2022

Please see [KLRNucleationMicroForce](source/materials/KLRNucleationMicroForce.md)

### KLBF (Kumar, Lopez, Bourdin, Francfort) Model 2020

Please see [KLBFNucleationMicroForce](source/materials/KLBFNucleationMicroForce.md)

## Example Input File Syntax

```
[Materials]
  [micro_force]
    type = LDLNucleationMicroForce
    regularization_length = l
    normalization_constant = c0
    tensile_strength = sigma_ts
    hydrostatic_strength = sigma_hs
    external_driving_force_name = ce
    h_correction = true
  []
[]
```

!syntax parameters /Materials/LDLNucleationMicroForce

!syntax inputs /Materials/LDLNucleationMicroForce

!syntax children /Materials/LDLNucleationMicroForce
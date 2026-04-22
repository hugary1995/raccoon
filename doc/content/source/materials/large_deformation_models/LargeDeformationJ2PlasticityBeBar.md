# LargeDeformationJ2PlasticityBeBar

!syntax description /Materials/LargeDeformationJ2PlasticityBeBar

## Overview

This class implements large deformation $J_2$ plasticity using the corrected modified left Cauchy-Green tensor ($\bar{\mathbf{b}}^e$) return-mapping algorithm of [!cite](bordenPhasefieldFormulationFracture2016). It requires the [CNHIsotropicElasticity](CNHIsotropicElasticity.md) model.

### Kinematics

The elastic left Cauchy-Green tensor $\mathbf{b}^e = \mathbf{F}^e {\mathbf{F}^e}^T$ admits the multiplicative split $\mathbf{F} = \mathbf{F}^e \mathbf{F}^p$. The isochoric part is tracked via

\begin{equation}
  \bar{\mathbf{b}}^e = J^{-2/3} \mathbf{b}^e, \qquad \det\bigl[\bar{\mathbf{b}}^e\bigr] = 1,
\end{equation}

and its incremental update uses the volume-preserving part of the relative deformation gradient,

\begin{equation}
  \bar{\mathbf{f}}_{n+1} = \text{det}[\mathbf{f}_{n+1}]^{-1/3}\mathbf{f}_{n+1}, \qquad \mathbf{f}_{n+1} = \mathbf{F}_n^{-1} \mathbf{F}_{n+1},
\end{equation}

so that the trial state is $\bar{\mathbf{b}}^e_{\mathrm{tr}} = \bar{\mathbf{f}} \, \bar{\mathbf{b}}^e_n \, \bar{\mathbf{f}}^T$.

### Return mapping

The yield function takes the form

\begin{equation}
  \phi = \|\mathbf{s}_{\mathrm{tr}}\| - \sqrt{\tfrac{2}{3}}\left(\psi^p_{,\bar\varepsilon^p} + \psi^{p*}_{,\Delta\bar\varepsilon^p}\right) - \sqrt{\tfrac{2}{3}}\, g_e G \,\mathrm{tr}[\bar{\mathbf{b}}^e] \,\Delta\bar\varepsilon^p,
\end{equation}

where $\mathbf{s}_{\mathrm{tr}} = g_e G \,\mathrm{dev}[\bar{\mathbf{b}}^e_{\mathrm{tr}}]$ is the trial deviatoric Kirchhoff stress, $g_e$ is the elastic degradation function, and $G$ is the shear modulus. If $\phi > 0$, a radial-return solve determines the plastic strain increment $\Delta\bar\varepsilon^p$.

### Determinant correction

After the stress update, the deviatoric part of $\bar{\mathbf{b}}^e$ is known but its trace (i.e. $\bar{I}^e$) must be found to enforce $\det[\bar{\mathbf{b}}^e] = 1$. Following [!cite](borden2016phase), this is done by solving,

\begin{equation}
  \text{det}\left[\text{dev}\left[\bar{\mathbf{b}}_{n+1}^e\right]+\bar{I}_{n+1}^e\mathbf{I}\right] = 1,
\end{equation}

and then updating $\bar{\mathbf{b}}^e$,

\begin{equation}
  \bar{\mathbf{b}}_{n+1}^e = \mathrm{dev}[\bar{\mathbf{b}}_{n+1}^e] + \bar{I}_{n+1}^e \mathbf{I}.
\end{equation}

This correction step distinguishes `LargeDeformationJ2PlasticityBeBar` from [LargeDeformationJ2Plasticity](LargeDeformationJ2Plasticity.md).

### Strain energy density

The compressible neo-Hookean strain energy density is split into volumetric and deviatoric parts,

\begin{equation}
  \psi^e = \underbrace{\tfrac{K}{2}\!\left[\tfrac{1}{2}(J^2-1) - \ln J\right]}_{U(J)} + \underbrace{\tfrac{G}{2}\!\left[\mathrm{tr}(\bar{\mathbf{b}}^e) - 3\right]}_{W(\bar{\mathbf{b}}^e)}.
\end{equation}

When `apply_strain_energy_split = true` (default), the active energy driving fracture is $U + W$ for $J \geq 1$ and $W$ only for $J < 1$ (compression excluded). The full energy $\psi^e$ is degraded by $g_e(d)$.

!syntax parameters /Materials/LargeDeformationJ2PlasticityBeBar

!syntax inputs /Materials/LargeDeformationJ2PlasticityBeBar

!syntax children /Materials/LargeDeformationJ2PlasticityBeBar

## References

!bibtex bibliography

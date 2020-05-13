# The Helmholtz free energy

!row!

!media media/potato_intact.png
       style=width:32%;float:left;
       id=potato_intact
       caption=a potato

!media media/potato_broken.png
       style=width:32%;float:left;margin-left:1%;margin-right:1%;
       id=potato_broken
       caption=a potato with a sharp crack

!media media/potato_diffuse.png
       style=width:32%;float:left;
       id=potato_diffuse
       caption=a potato with a diffuse crack

!row-end!

We present here a general form of the phase field PDEs, and then show how it can be solved via
FEM. The total free energy of a conservative system ([potato_intact]) may be written as

\begin{equation}
  \Psi_\total = -\Psi_\external + \Psi_\internal
\end{equation}

Now suppose the local response at certain material points exceeds the fracture strength and the excess of internal energy forms a crack represented by an internal crack set $\Gamma$ ([potato_broken]). For a closed system, the energy can now be written as:

\begin{equation}\label{eq.total_energy}
  \Psi_\total = -\Psi_\external + \left( \Psi_\internal - \Psi_\fracture \right) + \Psi_\fracture~,
\end{equation}

where $\Psi_\fracture$ is the fracture energy associated with the crack set $\Gamma$, which is governed by a well-established material-dependent fracture toughness $\Gc$:
\begin{equation}\label{eq.strong_fracture_energy}
    \Psi_\fracture := \int\limits_\Gamma \Gc \diff{A}~.
\end{equation}

With a view towards discretization, treating such a boundary integral requires an explicit crack surface tracking algorithm as well as a method to handle the resulting discontinuities in the displacement field.  The phase-field approach circumvents these issues by regularizing the strong discontinuity across the crack surfaces using a crack surface density function ([potato_diffuse]), and thereby approximating the surface integral [eq.strong_fracture_energy] with a volume integral:
\begin{equation}\label{eq.weak_fracture_energy}
    \Psi_\fracture \approx \widetilde{\Psi}_\fracture = \int\limits_\Omega \Gc \gamma \diff{V}~,
\end{equation}
where $\gamma$ is the crack-density function to be defined.
Substituting the approximation to the fracture energy into the total energy balance [eq.total_energy] yields
\begin{equation}
  \begin{aligned}
    \Psi_\total & = -\widetilde{\Psi}_\external + \left( \Psi_\internal - \widetilde{\Psi}_\fracture \right) + \widetilde{\Psi}_\fracture~, \\
    & = -\widetilde{\Psi}_\external + \widetilde{\Psi}_\internal + \widetilde{\Psi}_\fracture~.
  \end{aligned}
\end{equation}
What remains is a precise definition for the crack surface density function $\gamma$ as well as the degraded internal energy $\widetilde{\Psi}_\internal$ in terms of the phase-field $d$.  

## Approximation to the crack surface energy

We use the Allen-Cahn approximation to the crack surface, assuming a smooth transition from $d = 1$ along the crack set to $d = 0$ away from the crack set, i.e.
\begin{equation}\label{eq.AC_crack_surface_density}
    \gamma(d;l) := \dfrac{1}{c_0l} \left( w(d) + l^2 \norm{\nabla d}^2 \right) \quad \text{with} \quad c_0 = 4 \int\limits_0^1 \sqrt{w (s)}\text{ d}s~,
\end{equation}
where $w(d) \in C^2([0,1])$ is the local dissipation function and $l$ is a regularization parameter carrying units of length. The normalization constant $c_0$ is chosen such that, for $d \in [0,1]$,
\begin{equation}
    \lim_{l \rightarrow 0} \int\limits_\Omega \Gc\gamma(d;l) \diff{V} = \int\limits_\Gamma \Gc \diff{A} .
\end{equation}

## Degradation of the internal energy

The phase-field variable $d$ is often interpreted as the "damage" because it is employed both within a geometric context, i.e. [eq.AC_crack_surface_density], and also to couple the fracture energy and the degraded internal energy, via
\begin{equation}\label{eq.general_energy_split}
  \begin{aligned}
    \Psi_\internal             & = \int\limits_\Omega \psi_\internal \diff{V} = \int\limits_\Omega \psi_\internal^\activeenergy \diff{V} + \int\limits_\Omega \psi_\internal^\inactiveenergy \diff{V},\\
    \widetilde{\Psi}_\internal & = \int\limits_\Omega g(d)\psi_\internal^\activeenergy \diff{V} + \int\limits_\Omega \psi_\internal^\inactiveenergy \diff{V}~,
  \end{aligned}
\end{equation}
where $g(d) \in C^2([0,1])$ is the degradation function. Intuitively, $g(1) = 0$ for fully damaged material, and $g(0) = 1$ for intact material. The internal energy density is split into active and inactive parts [eq.general_energy_split], such that only the active internal energy is associated with fracture, and the degraded internal energy is the sum of degraded active internal energy and intact inactive internal energy.

## Solution

Finally, the system can be solved by minimizing the total energy:
\begin{equation}
  \begin{aligned}
    & \min_{\bs{u},d} \left( -\widetilde{\Psi}_\external + \widetilde{\Psi}_\internal + \widetilde{\Psi}_\fracture \right)\\
    & \quad \text{subject to } \dot{d} \geqslant 0~,
  \end{aligned}
\end{equation}
where $\bs{u}$ represents all variables other than $d$ in order to completely define the state of the system. The monotonicity constraint imposes the irreversibility of the damage process, consistent with the assumption that fractured surfaces do not "heal". The minimizers are discussed [here](theory/minimizer.md)

# Thermodynamics

The rates of change of the kinematic variables are denoted as $\mathcal{V} = \{ \dot{\bs{\phi}}, \dot{\bfF}^p, \epdot, \dot{d} \}$. The generalized velocities of the kinematic state variables and their corresponding thermodynamic force conjugates are collected in the sets
\begin{equation}
  \begin{aligned}
    \dot{\Lambda} = \{ \grad \dot{\bs{\phi}}, \dot{\bfF}^p, \epdot, \dot{d}, \grad \dot{d} \}, \quad \mathcal{F} = \{ \bfP, \bfT, Y, f, \bs{\xi} \}.
  \end{aligned}
\end{equation}

The internal power expenditure per unit volume takes the form
\begin{equation}
  \begin{aligned}
    \mathcal{P}_\text{int} = \bfP:\dot{\bfF} + \bfT:\dot{\bfF}^p + Y\epdot + f\dot{d} + \bs{\xi}\cdot\grad\dot{d},
  \end{aligned}
\end{equation}
and the external power expenditure $\mathcal{P}_\text{ext}$ is assumed to be the sum of surface traction, body force, and the so-called micro-forces.

Let $\psi$ be the Helmholtz free energy per unit volume in the reference configuration. Assuming no external volumetric heat source, the total entropy rate, the internal entropy production rate and the external entropy input rate per unit volume can be rearranged as
\begin{equation}
  \begin{aligned}
    \rho T \dot{s}            & = \rho T \dot{s}_\text{int} + \rho T \dot{s}_\text{ext},                                         \\
    \rho T \dot{s}_\text{int} & \equiv \mathcal{P}_\text{int} - \dot{\psi} - \dfrac{1}{T}\bfq\cdot\grad T \geqslant 0,  \\
    \rho T \dot{s}_\text{ext} & \equiv - \divergence \bfq + \dfrac{1}{T}\bfq\cdot\grad T,
  \end{aligned}
\end{equation}
where $\rho$ is the reference density, and $\bfq$ is the heat flux. Substituting the internal power expenditure into the internal entropy rate yields the local dissipation inequality
\begin{equation}
  \begin{aligned}
    \rho T \dot{s}_\text{int} = \bfP:\dot{\bfF} + \bfT:\dot{\bfF}^p + Y\epdot + f\dot{d} + \bs{\xi}\cdot\grad\dot{d} - \dot{\psi} - \dfrac{1}{T}\bfq\cdot\grad T \geqslant 0.
  \end{aligned}
\end{equation}

We assume that the generalized forces can be additively decomposed into an energetic part and a dissipative part. That is
\begin{equation}
  \begin{aligned}
    \bfP &= \bfP^\text{en}(\Lambda) + \bfP^\text{dis}(\dot{\bfF}; \Lambda), \\
    \bfT &= \bfT^\text{en}(\Lambda), \\
    Y &= Y^\text{en}(\Lambda) + Y^\text{dis}(\epdot; \Lambda), \\
    f &= f^\text{en}(\Lambda) + f^\text{dis}(\dot{d}; \Lambda), \\
    \bs{\xi} &= \bs{\xi}^\text{en}(\Lambda).
  \end{aligned}
\end{equation}
The energetic forces depend on the current thermodynamic state, while the dissipative forces depend only on the conjugate rate variables. Apparently, the dissipative forces must vanish in the quasi-static setting as the generalized velocities diminish.

Let $a_{,b}$ denote the partial derivative of $a$ with respect to $b$. The time derivative of the free energy density can be written as
\begin{equation}
  \begin{aligned}
    \dot{\psi} = \psi_{, \bfF}:\dot{\bfF} + \psi_{, \bfF^p}:\dot{\bfF}^p + \psi_{, \ep}\epdot + \psi_{, d}\dot{d} + \psi_{, \grad d}\cdot\grad\dot{d}.
  \end{aligned}
\end{equation}
Substituting the rate of free energy into the dissipation inequality and applying the Coleman-Noll procedure lead to restrictions on the constitutive relations:
\begin{equation}
  \begin{aligned}
    \bfP^\text{en} = \psi_{, \bfF}, \quad \bfT^\text{en} = \psi_{, \bfF^p}, \quad Y^\text{en} = \psi_{, \ep}, \quad f^\text{en} = \psi_{, d}, \quad \bs{\xi}^\text{en} = \psi_{, \grad d},
  \end{aligned}
\end{equation}
and the dissipation inequality reduces to
\begin{equation}
  \begin{aligned}
    \rho T \dot{s}_\text{int} = P^\text{dis}:\dot{\bfF} + Y^\text{dis}\epdot + f^\text{dis}\dot{d} - \dfrac{1}{T}\bfq\cdot\grad T \geqslant 0.
  \end{aligned}
\end{equation}

Substituting the dissipation inequality and the external entropy rate into the total entropy rate, and using the definition of entropy $s = -\rho\psi_{, T}$ and heat capacity $c \equiv -\rho T \psi_{, TT}$, the heat equation can be written as
\begin{equation}
  \begin{aligned}
    \rho T \dot{s} = \rho c \dot{T} = - \divergence\bfq + P^\text{dis}:\dot{\bfF} + Y^\text{dis}\epdot + f^\text{dis}\dot{d}.
  \end{aligned}
\end{equation}

!content pagination use_title=True
                    previous=theory/kinematics.md
                    next=theory/minimization.md

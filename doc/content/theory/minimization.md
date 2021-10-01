# The minimization problem

We construct a potential such that all the thermodynamic equations of state can be derived from a minimization problem. To maintain such variational structure, all dissipative force conjugates are assumed to derive from dual kinetic potentials:
\begin{equation}
  \begin{aligned}
    \Delta = R(\dot{\bfF}; \Lambda) + \Pi(\epdot; \Lambda) + \Phi(\dot{d}; \Lambda), \quad \bfP^\text{dis} = R_{, \dot{\bfF}}, \quad Y^\text{dis} = \Pi_{, \epdot}, \quad f^\text{dis} = \Phi_{, \dot{d}}.
  \end{aligned}
\end{equation}
To satisfy the second law, the material is assumed to be *strictly dissipative* in the sense that every thermodynamic process results in an increase in entropy for nonzero rates.

The free energy density $\psi$ is additively decomposed into the strain energy density, the plastic energy density, and the fracture energy density:
\begin{equation}
  \begin{aligned}
    \psi = \psi^e(\bfF, \bfF^p, d) + \psi^p(\ep, d) + \psi^f(d, \grad d).
  \end{aligned}
\end{equation}
Considering external power expenditure (body force $\bfb$ and surface traction $\bft$), the total potential can be constructed as
\begin{equation}
  \begin{aligned}
    L(\dot{\bs{\phi}}, \dot{\bfF}^c, \epdot, \dot{d}) = \int\limits_{\body_0} \left( \dot{\psi} + \Delta \right) \diff{V} - \int\limits_{\bodyboundary} \bft\cdot\dot{\bs{\phi}} \diff{A} - \int\limits_{\body} \bfb\cdot\dot{\bs{\phi}}.
  \end{aligned}
\end{equation}
Substitute the definition of the free energy, the definition of the dual kinetic potential and the constitutive restrictions into the total potential to obtain
\begin{equation}
  \begin{aligned}
    L(\dot{\bs{\phi}}, \bfN^c, \epdot, \dot{d}) = \int\limits_{\body_0} \left( \bfP^\text{en}:\dot{\bfF} + \bfT:\dot{\bfF}^p + f^\text{en}\dot{d} + \bs{\xi}\cdot\grad d + R + \Pi + \Phi \right) \diff{V}.
  \end{aligned}
\end{equation}
The thermodynamic equations of state can then be derived by finding the infimum of the total potential, subject to flow rule constraint, the irreversibility conditions, and Dirichlet boundary conditions.

The balance of linear momentum is obtained by minimizing $L$ with respect to $\dot{\bs{\phi}}$:
\begin{equation}
  \begin{aligned}
    \divergence (\bfP^\text{en}+\bfP^\text{dis}) + \bfb = \bs{0}.
  \end{aligned}
\end{equation}

Minimization of $L$ jointly with respect to $\epdot$ and $\dot{\bfF}^p$, subject to flow rule constraints, determines the flow direction and the plastic loading/unloading conditions.
\begin{equation}
  \begin{aligned}
    \phi^p \leqslant 0, \quad \epdot \geqslant 0, \quad \phi^p\epdot = 0,
  \end{aligned}
\end{equation}

Finally, the minimization with respect to $\dot{d}$ leads to the so-called fracture evolution equations
\begin{equation}
  \begin{aligned}
    \phi^f \equiv \divergence \bs{\xi} - (f^\text{en} + f^\text{dis}) \leqslant 0, \quad \dot{d} \geqslant 0, \quad \phi^f\dot{d} = 0.
  \end{aligned}
\end{equation}

!content pagination use_title=True
                    previous=theory/thermodynamics.md
                    next=theory/constitutive_models.md

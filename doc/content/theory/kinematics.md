# State variables and kinematics

We start by defining degrees of freedom in the system. Let $\body_0$ be the reference configuration and $\Omega_t$ be the current configuration at time $t$. Let $\bs{\phi} : \bfX \to \bfx$ be the deformation map from $\bfX \in \body_0$ to $\bfx \in \body_t$. The deformation gradient is denoted as $\bfF = \grad\bs{\phi}$, where the operator $\grad$ denotes differentiation with respect to $\bfX$. The deformations due to thermal expansion and plastic flow are modeled using the multiplicative decomposition

\begin{equation}
  \bfF = \bfF^e\bfF^p\bfF^g,
\end{equation}

where $\bfF^g$, $\bfF^e$, and $\bfF^p$ are referred to as the eigen, elastic, and plastic deformation gradients, respectively. For convenience, $\bfF^m = \bfF^e\bfF^p$ is defined as the mechanical deformation gradient.

In general, there exist constraints for the plastic flow and the effective plastic strain rate $\ep$ of the following form:

\begin{equation}
  \mathcal{Z}(\dot{\bfF}^p{\bfF^p}^{-1}, \epdot) = 0
\end{equation}

For example, the Prandtl-Reuss flow rule requires the plastic flow to be purely isochoric and normalized so that the plastic deformation is uniaxial:

\begin{equation}
  \begin{aligned}
    \tr\left( \dot{\bfF}^p{\bfF^p}^{-1} \right) &= 0, \\
    \norm{\dot{\bfF}^p{\bfF^p}^{-1}}^2 - \dfrac{3}{2}\abs{\epdot}^2 &= 0.
  \end{aligned}
\end{equation}

In contrast, Tresca's flow rule is more restrictive in the sense that the plastic flow may only occur along the crystal slip system, i.e. $\bfs\otimes\bfm$:

\begin{equation}
  \dot{\bfF}^p{\bfF^p}^{-1} = \epdot \bfs\otimes\bfm.
\end{equation}

In the phase-field approach to fracture, crack surfaces are regularized and modeled using a phase-field $d$. Both the plastic flow and the crack evolution are considered irreversible, i.e.

\begin{equation}
  \epdot \geqslant 0, \quad \dot{d} \geqslant 0.
\end{equation}

The set of state variables are $\mathcal{K} = \{ \bs{\phi}, \bfF^p, \ep, d \}$.

!content pagination use_title=True
                    previous=theory/intro.md
                    next=theory/thermodynamics.md

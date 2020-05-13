## The minimizer

To solve the energy minimization problem [discussed earlier](theory/free_energy.md), we first find the necessary conditions for the minimizer of the objective function by temporarily relaxing the constraint, i.e. by solving the unconstrained version as:

\begin{equation}
    \min_{\bs{u},d} \widetilde{\Psi}_\total = -\widetilde{\Psi}_\external + \widetilde{\Psi}_\internal + \widetilde{\Psi}_\fracture~.
\end{equation}

The Euler-Lagrange equations for this unconstrained minimization problem state that the minimizer satisfies:

\begin{equation}\label{eq.Euler-Lagrange}
    \begin{aligned}
        \divergence \dfrac{\delta \Psi_\total(\bs{u},\nabla\bs{u},\dot{\bs{u}})}{\delta \nabla\bs{u}} - \dfrac{\delta \Psi_\total(\bs{u},\nabla\bs{u},\dot{\bs{u}})}{\delta \bs{u}} - \dfrac{\text{d}}{\text{d}t}\dfrac{\delta \Psi_\total(\bs{u},\nabla\bs{u},\dot{\bs{u}})}{\delta \dot{\bs{u}}} & = \bs{0} , \\
        \divergence \dfrac{\delta \Psi_\total(d,\nabla d,\dot{d})}{\delta \nabla d} - \dfrac{\delta \Psi_\total(d,\nabla d,\dot{d})}{\delta d} - \dfrac{\text{d}}{\text{d}t}\dfrac{\delta \Psi_\total(d,\nabla d,\dot{d})}{\delta d}                                                               & = 0 .
    \end{aligned}
\end{equation}

For the quasi-static fracture problems, the total energy can be written in terms of displacements $\bs{u}$ and damage $d$ as:

\begin{equation}\label{eq.instantiated_free_energy}
    \begin{aligned}
        \Psi_\total = & \underbrace{\int\limits_{\partial\Omega} \bs{\tau} \cdot \bs{u} \diff{A}}_{\text{external energy}} + \underbrace{\int\limits_\Omega g(d)\psi_\elastic^\activeenergy \diff{V} + \int\limits_\Omega \psi_\elastic^\inactiveenergy \diff{V}}_{\text{degraded elastic energy}} - \underbrace{\int\limits_\Omega \bs{b} \cdot \bs{u} \diff{V}}_{\text{potential energy}} \\ & + \underbrace{\int\limits_\Omega \dfrac{3\mathcal{G}_c}{8l} \left( w(d) + l^2 \norm{\nabla d}^2 \right) \diff{V}}_{\text{approx. fracture energy}},
    \end{aligned}
\end{equation}

where $\bs{\tau}$ is the surface traction and $\bs{b}$ is the body force. Substituting [eq.instantiated_free_energy] into [eq.Euler-Lagrange], we obtain the necessary conditions of the minimizer, which are essentially the governing equations of the given system together with boundary conditions:

\begin{equation}
  \begin{aligned}
    \divergence \xstress + \bs{b}                                                       & = \bs{0} , \quad \text{in }\Omega~,\\
    \kappa\Delta d - \left( w'(d) + \dfrac{g'(d)}{M}\psi_\elastic^\activeenergy \right) & = 0, \quad \text{in } \Omega~,\\
    \xstress \cdot \normal                                                              & = \bs{\tau}, \quad \text{on } {\partial\Omega}~,\\
    \kappa \nabla d \cdot \normal                                                        & = 0, \quad \text{on } {\partial\Omega}~,
  \end{aligned}
\end{equation}

where

\begin{equation}
    \xstress = g(d)\dfrac{\delta \psi_\elastic^\activeenergy(\strain(\nabla\bs{u}))}{\delta \nabla\bs{u}} + \dfrac{\delta \psi_\elastic^\inactiveenergy(\strain(\nabla\bs{u}))}{\delta \nabla\bs{u}}~,
\end{equation}

is the degraded stress tensor, $\kappa = 2l^2$ is the interfacial coefficient, and $M = \dfrac{3\mathcal{G}_c}{8l}$ is often referred to the mobility in keeping with general Allen-Cahn phase-field models. The damage begins to accumulate as soon as:

\begin{equation}
    \psi_\elastic^\activeenergy > \psi_\critical = -M\dfrac{w'(0)}{g'(0)} ,
\end{equation}

where $\psi_\critical$ is a model-specific energy threshold for damage initiation. For example, a brittle fracture model that uses $w(d) = d^2$ results in a critical energy $\psi_\critical = 0$, and cohesive-type fracture models that use $w(d) = d$ have a critical energy $\psi_\critical = -M/g'(0)$.  We adopt a cohesive fracture model that uses the local dissipation function $w(d) = d$ in conjunction with the degradation function proposed by Lorentz:

\begin{equation}
    g(d) = \dfrac{(1-d)^2}{(1-d)^2+\dfrac{M}{\psi_\critical}d(1+pd)} ,
\end{equation}

such that the critical fracture energy is not necessarily correlated with the other material properties and model parameters, i.e. the fracture toughness $\Gc$ and the length scale $l$.

## The fixed-point iterative solution scheme

To solve the non-convex [minimization problem](theory/free_energy.md), we adopt the alternating minimization (alternating directions) strategy. The full minimization problem is split into two subproblems:
\begin{equation}
  \begin{aligned}
    &(\text{P1}):\quad \min\limits_{\bs{u}} \left( -\widetilde{\Psi}_\external + \widetilde{\Psi}_\internal + \widetilde{\Psi}_\fracture \right), \text{and}\\
    &(\text{P2}): \quad \begin{cases}
        &\min\limits_{d} \left( -\Psi_\external + \widetilde{\Psi}_\internal + \widetilde{\Psi}_\fracture \right) \\
        &\text{subject to } \dot{d} \geqslant 0 .
    \end{cases}
  \end{aligned}
\end{equation}
In other words, the first subproblem (P1) solves for $\bs{u}$ with a +frozen+ damage field $d$, and the second subproblem (P2) solves for the damage $d$ with a +frozen+ fracture driving energy. Since the objective function is convex and closed in $\bs{u}$ and $d$ respectively, the alternating minimization technique is able to solve the two subproblems iteratively until convergence. The solution algorithm is summarized in Algorithm 1.

!media media/fpi.png
       style=width:100%;margin:auto;padding-top:2.5%;

## Weak form and finite element discretization

The solutions to the necessary conditions of the objective functions, i.e., the governing equations, are approximated using the finite element method. We now describe the discretization of the phase-field evolution equation. Let the trial and weighting spaces be $\mathcal{D}_t$ and $\mathcal{Q}$:
\begin{equation}
    \mathcal{D}_t = \{ d \in \mathcal{H}^1(\Omega) \}, \quad \mathcal{Q} = \{ q \in \mathcal{H}^1(\Omega) \},
\end{equation}
followed by the weak form:
\begin{equation}
    \left<W\right>
    \begin{cases}
        \text{Given $\bs{u}_{n,a+1}$, at $t \in [t_n,t_{n+1}]$, find $d_{n,a+1} \in \mathcal{D}_t$, such that } \forall q \in \mathcal{Q},\\
        \quad -(M \nabla q, \kappa \nabla d_{n,a+1})_\Omega - \left( q, g'(d_{n,a+1})\psi_\elastic^\activeenergy (\bs{u}_{n,a+1}) \right)_\Omega - (q, M)_\Omega = 0, \\
        \quad \text{subject to } \norm{\macaulay{d_{n,a+1} - d_n}_-} = 0 .
    \end{cases}
\end{equation}
Using the Galerkin method, with finite dimensional function spaces $\widetilde{\mathcal{D}}_t \subset \mathcal{D}_t$, $\widetilde{\mathcal{Q}} \subset \mathcal{D}$, we arrive at the Galerkin form of the problem:
\begin{equation}
    \left<G\right>
    \begin{cases}
        \text{Given $\bs{u}_{n,a+1}$, at $t \in [t_n,t_{n+1}]$, find $d_{n,a+1} \in \widetilde{\mathcal{D}}_t$, such that }\forall q \in \widetilde{\mathcal{Q}},\\
        \quad -(M \nabla q, \kappa \nabla d_{n,a+1})_\Omega - \left( q, g'(d_{n,a+1}) \psi_\elastic^\activeenergy (\bs{u}_{n,a+1}) \right)_\Omega - (q, M)_\Omega = 0, \\
        \quad \text{subject to } \norm{\macaulay{d_{n,a+1} - d_n}_-} = 0 .
    \end{cases}
\end{equation}

RACCOON is designed bottom-up to make the implementation of this weak form modular and general. Each material or kernel definition is designated for a small portion of the entire equation, and they are coupled together to solve the entire evolution equation. Below is a table listing the terms and the corresponding kernels:

| Term                                                                                    | Kernel                                                                             |
| --------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------- |
| $(M \nabla q, \kappa \nabla d_{n,a+1})_\Omega$                                          | [`PhaseFieldFractureEvolutionDiffusion`](/PhaseFieldFractureEvolutionDiffusion.md) |
| $\left( q, g'(d_{n,a+1}) \psi_\elastic^\activeenergy (\bs{u}_{n,a+1})+M \right)_\Omega$ | [`PhaseFieldFractureEvolutionReaction`](/PhaseFieldFractureEvolutionReaction.md)   |

Below is a table listing the materials:

| Term     | Parameters        | Material                                                     |
| -------- | ----------------- | ------------------------------------------------------------ |
| $M$      | $\Gc$, $l$, $c_0$ | [`FractureMaterial`](/FractureMaterial.md)                   |
| $\kappa$ | $l$               | [`FractureMaterial`](/FractureMaterial.md)                   |
| $g(d)$   | $d$               | [`QuadraticDegradation`](/QuadraticDegradation.md)           |
| $g(d)$   | $d$, $M$          | [`LorentzDegradation`](/LorentzDegradation.md)               |
| $w(d)$   | $d$               | [`LinearLocalDissipation`](/LinearLocalDissipation.md)       |
| $w(d)$   | $d$               | [`QuadraticLocalDissipation`](/QuadraticLocalDissipation.md) |

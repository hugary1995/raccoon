# Introduction to Phase Field

In the phase field approach, microstructural features are described using continuous variables.
These variables take two forms: conserved variables representing physical properties such as atom
concentration or material density, and nonconserved order parameters describing the microstructure of
the material, including grains and different phases.  The evolution of these continuous variables is
a function of the Gibbs free energy and can be defined as a system of partial differential equations
(PDEs). Thus, to define the kinetics of the system, the free energy must be described as a function
of the continuous variables representing the microstructure.

The system of PDEs representing the evolution of the various variables required to represent a given
system and the free energy functional comprise a specific phase field model. The model may also be
coupled to additional physics, such as mechanics or heat conduction. These model equations may be
solved in many ways, including finite difference, spectral methods, and the finite element method
(FEM).

Phase field for fracture model may be viewed as an application of the general phase field equations.
Excessive elastic energy is degraded and transformed into a fracture surface energy represented by
nonconserved order parameters $d_i$. RACCOON provides the necessary tools to rapidly develop a phase field
for fracture simulation tool with the equations solved via FEM.

## Allen-Cahn Equation

We present this a general form of the phase field PDEs here, and then show how it can be solved via
FEM. The PDE's are evolution equations for the various variables and are functions of the free
energy functional $F$. The evolution of nonconserved order parameters $d_i$ is represented with an Allen-Cahn
equation, according to

\begin{equation}\label{eq:AC}
\beta \dot{d_i} = \nabla\cdot M_i\nabla(\kappa_id_i) - M_i(\frac{\partial f_{loc}}{\partial d_i} + \frac{\partial E_{add}}{\partial d_i})
\end{equation}

where $d_i$ is an order parameter (interpreted as damage in a fracture setting) and $M_i$ is the order parameter mobility, $\kappa_i$ is the interface coefficient.

The weak form follows as:
\begin{equation}\label{eq: weak}
\boldsymbol{\mathcal{R}}_{d_i} = \left(\psi, \beta\dot{d_i}\right) + \left( \nabla\psi, M_i\nabla(\kappa_id_i) \right) + M_i \left( \psi, \frac{\partial f_{loc}}{\partial d_i} + \frac{\partial E_{add}}{\partial d_i} \right)
\end{equation}

## Phase Field for Fracture

For a typical mechanical-fracture coupled system, the total free energy can be expressed as:
\begin{equation}
\begin{aligned}
F &= g(d)F_e^+ + F_e^- + F_{frac}\\
g(d) &= \Pi_i g(d_i)\\
F_{frac} &= G_c\Gamma = \sum_i G_{ci}\int_\Omega \gamma(d_i,\nabla d_i)d\Omega
\end{aligned}
\end{equation}

where $g(d)$ is the lumped degradation function of all the damage parameters, $G_c$ is the fracture energy release rate, and
$\gamma$ is a fracture surface energy density function.

The general form of $\gamma$ can be written as:
\begin{equation}
\gamma(d,\nabla d) = \frac{1}{4||w(d)||}(\frac{1}{L}w(d)+L\nabla d \cdot \nabla d)
\end{equation}

To minimize the total free energy, Euler-Lagrange equation states that:
\begin{equation}
\begin{aligned}
\beta \dot{d_i} &= \nabla\cdot\frac{\partial F}{\partial \nabla d_i}-\frac{\partial F}{\partial d_i}\\
&= \nabla\cdot\frac{G_{ci}}{4||w(d)||}(2L_i\nabla d_i)-\frac{\partial g(d)F_e^+}{\partial d_i}-\frac{G_{ci}}{4||w(d)||}\frac{1}{L_i}\frac{\partial w(d_i)}{\partial d_i}\\
&= \nabla\cdot\frac{G_{ci}}{4||w(d)||L_i}(2L_i^2\nabla d_i)-\frac{\partial g(d)F_e^+}{\partial d_i}-\frac{G_{ci}}{4||w(d)||L_i}\frac{\partial w(d_i)}{\partial d_i}
\end{aligned}
\end{equation}

compare with [eq:AC], we see that if we define $\kappa_i$ and $M_i$ as:
\begin{equation}
\begin{aligned}
\kappa_i &= 2L_i^2\\
M_i &= \frac{G_{ci}}{4||w(d)||L_i}
\end{aligned}
\end{equation}

The fracture evolution equation can be rewritten in terms of interface coefficient $\kappa_i$ and mobility $M_i$:
\begin{equation}
\begin{aligned}
\beta \dot{d_i} &= \nabla\cdot M_i(\nabla \kappa_id_i)-\frac{\partial g(d)F_e^+}{\partial d_i}-M_i\frac{\partial w(d_i)}{\partial d_i}\\
&= \nabla\cdot M_i(\nabla \kappa_id_i)-M_i\left(\frac{1}{M_i}\frac{\partial g(d)F_e^+}{\partial d_i}+\frac{\partial w(d_i)}{\partial d_i}\right)
\end{aligned}
\end{equation}

Then, it is trivial to see that the evolution equation for phase field fracture is simply an application of the Allen-Cahn equation for nonconserved order parameter.
It is convenient to interpret the phase field fracture evolution equation as [eq:AC] with the substitutions of:
\begin{equation}
\begin{aligned}
f_{loc} &= w(d_i)\\
F_{add} &= \frac{g(d)F_e^+}{M_i}
\end{aligned}
\end{equation}

## Weak Form

Weak form of the fracture evolution equation is:
\begin{equation}
\boldsymbol{\mathcal{R}}_{d_i} = \left(\psi, \beta\dot{d_i}\right) + \left( \nabla\psi, M_i\nabla(\kappa_id_i) \right) + M_i \left( \psi, \frac{\partial \left[w(d_i)+\frac{g(d)F_e^+}{M_i}\right]}{\partial d_i} \right)
\end{equation}

RACCOON is designed bottom-up to make the implementation of this weak form modular and general. Each material or kernel definition is designated for a small portion of the entire equation, and they are coupled together to solve the entire evolution equation. Below is a table listing the functionality of each class and the terms/properties it is responsible for.

| Residual term | Variable | Parameters | Energy derivative | Kernel |
| - | - | - | - | - |
$\left(\psi, \beta\dot{d_i}\right)$ | $d_i$ | | | [`MaterialTimeDerivative`](/MaterialTimeDerivative.md) |
$\left( \nabla\psi, M_i\nabla(\kappa_id_i) \right)$ | $d_i$ | | | `ACInterface` |
$M_i ( \psi, \frac{\partial \left[w(d_i)+\frac{g(d)F_e^+}{M_i}\right]}{\partial d_i} )$ | $d_i$ | | | `AllenCahn` |
 | | $\kappa_i,M_i$ | | [`PFFracBulkMaterial`](/PFFracBulkMaterial.md) |
 | | $g(d_i)$ | | [`Degradation`](/Degradation.md) |
 | | $g(d)$ | | [`LumpedDegradation`](/LumpedDegradation.md) |
 | | | $w(d_i)$ | [`LocalDissipation`](/LocalDissipation.md) |
 | | | $F_e^+$ | [`ComputeDegradedStress`](/ComputeDegradedStress.md) |
 | | | $w(d_i)+\frac{g(d)F_e^+}{M_i}$ | [`FractureDrivingForce`](/FractureDrivingForce.md) |

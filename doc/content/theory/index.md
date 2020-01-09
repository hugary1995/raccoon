In the phase field approach, microstructural features are described using continuous variables.
These variables take two forms: conserved variables representing physical properties such as atom
concentration or material density, and nonconserved order parameters describing the microstructure of
the material, including grains and different phases.  The evolution of these continuous variables is
a function of the Helmholtz free energy and can be defined as a system of partial differential equations
(PDEs). Thus, to define the kinetics of the system, the free energy must be described as a function
of the continuous variables representing the microstructure.

The system of PDEs representing the evolution of the various variables required to represent a given
system and the free energy functional comprise a specific phase field model. The model may also be
coupled to additional physics, such as mechanics or heat conduction. These model equations may be
solved in many ways, including finite difference, spectral methods, and the finite element method
(FEM).

Phase field for fracture model may be viewed as an application of the general phase field equations.
Excessive elastic energy is transformed into a fracture surface energy represented by a
nonconserved order parameter $d$, which is often referred to as damage in this context. RACCOON provides
the necessary tools to rapidly develop a phase field for fracture simulation tool with the equations solved via FEM.

The theory and finite element implementation details are discussed as outlined below:

- [Helmholtz free energy](theory/free_energy.md)
- [Solution strategy](theory/minimizer.md)
- [implementation](theory/implementation.md)

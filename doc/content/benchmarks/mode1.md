# Mode I Crack Propagation

!media media/mode1_bcs.png
       style=width:30%;margin:auto;padding-top:2.5%;
       id=mode1_bcs
       caption=boundary conditions of the Mode-I problem

Consider a 1mm $\times$ 1mm plate with a pre-existing crack on the left side, loaded in uniaxial tension, as shown in [mode1_bcs]. We assume the plate to be composed of an isotropic material with Young's modulus $E = 2.1 \times 10^5$MPa, Poisson's ratio $\nu = 0.3$, fracture toughness $\Gc = 2.7$Jmm$^{-3}$, and critical strength $\sigma_c = 2.5 \times 10^3$MPa.
The plate is fixed along the bottom, and the top surface is displaced in the vertical direction.  Roller supports are also assumed to be in place, such that the horizontal displacement vanishes along the top surface. The plate is assumed to be sufficiently thick such that plane strain conditions hold.

The fixed-point iterative scheme is implemented using three input files.

- [master.i](benchmarks/mode1_geometric_notch/master.i) controls the load that is being shared among all multiapps
- [mechanical.i](benchmarks/mode1_geometric_notch/mechanical.i) defines and solves the elasticity subproblem
- [fracture.i](benchmarks/mode1_geometric_notch/fracture.i) defines and solves the phase-field subproblem

## Load control

The "master.i" input file controls the load step and transfers the load as well as the solutions between multiapps. Global parameters are defined at the very top of the input file:

!listing benchmarks/mode1_geometric_notch/master.i start=E = 2.1e5 end=dc = 0.6 include-end=True

The master app need not solve for anything, so we have

!listing benchmarks/mode1_geometric_notch/master.i block=Problem

The elasticity subproblem defined in "mechanical.i" is a MultiApp of master. The fixed-point iteration is applied by enabling subcycling of the mechanical subapp:

!listing benchmarks/mode1_geometric_notch/master.i block=MultiApps

!alert note
The global parameters are transferred to MultiApps via `cli_args`

`load` is a scalar variable defining the magnitude of the load in current load step. It is transferred to the mechanical MultiApp from master:

!listing benchmarks/mode1_geometric_notch/master.i block=Transfers/send_load

The mechanical (and the fracture) MultiApp then solves the field equations and transfers the damage back to master. The master App keeps track of the damage variable such that it is applied as the lower bound in the fracture MultiApp. To achieve that, the transfer block looks like

!listing benchmarks/mode1_geometric_notch/master.i block=Transfers/get_d_ref Transfers/send_d_ref

## The elasticity subproblem

The "mechanical.i" input file defines and solves the elasticity subproblem. Two kernels define the residual term $\xstress_{ij,j}$:

!listing benchmarks/mode1_geometric_notch/mechanical.i block=Kernels

!alert note
Upon convergence, residual at Dirichlet DoFs are volumetric reaction forces. Those values are saved in `fy` and are summed by a postprocessor to get the reaction force in y direction.

Boundary conditions are defined according to [mode1_bcs]:

!listing benchmarks/mode1_geometric_notch/mechanical.i block=BCs

At the end of each time step, the fracture driving energy is saved in an auxiliary variable:

!listing benchmarks/mode1_geometric_notch/mechanical.i block=AuxKernels

which is transferred to the fracture MultiApp to define the damage evolution equation:

!listing benchmarks/mode1_geometric_notch/mechanical.i block=Transfers/send_E_el

## The fracture subproblem

The "fracture.i" input file defines and solves the elasticity subproblem. Two kernels define the residual of the damage evolution equation:

!listing benchmarks/mode1_geometric_notch/fracture.i block=Kernels

!alert note
The fracture driving energy is defined by an auxiliary variable retrieved from the mechanical MultiApp `E_el`.

Irreversibility of damage is enforced as a variation inequality:

!listing benchmarks/mode1_geometric_notch/fracture.i block=Bounds

A variational inequality solver +must+ be used for the bound to be effective. For example, here we use the `vinewtonrsls` nonlinear solver to enforce irreversibility:

!listing benchmarks/mode1_geometric_notch/fracture.i start=petsc_options_iname end=petsc_options_value include-end=True

## Defining initial damage

Instead of a "meshed-in" initial crack, sometimes we may need initial imperfections in the form of an initial damage field. To do so, simply define the initial damage field in the master input file, e.g.

!listing benchmarks/mode1_initial_damage/master.i block=ICs

[CohesiveDamageIC](CohesiveDamageIC.md) and [BrittleDamageIC](BrittleDamageIC.md) are predefined initial conditions based on closed-form solutions.

## Fixed-point iteration: the other way

Recently, another way to perform fixed-point iteration has made its way into RACCOON. As opposed to the old way, the new system needs only one input file. Between fixed-point iterations within one load step, stateful material properties are advanced. Therefore, by "lagging" the fracture driving energy one step, we achieve effectively fixed-point iteration.

!listing benchmarks/fixed_point_iteration/mechanical_fracture_uo.i

## Visualizing the outputs

The following animations are generated using Paraview.

!media media/mode1.gif
       style=width:60%;margin:auto;padding-top:2.5%;

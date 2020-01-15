# Irreversibility

!syntax description /AuxKernels/Irreversibility

## Description

PETSc provides interface for specifying lower and upper bounds used in variational inequality solvers. This AuxKernel creates the corresponding vectors, `lower_bound` and `upper_bound`, required by PETSc in its constructor. At times requested by `execute_on` flags, it loops through DoFs specified by `variable` and fills in the lower and upper bounds.

In the current implementation, the upper bound `upper` is provided by a real number, and the lower bound `lower` is provided by a coupled variable. If `lag` is set to true, then the old value of the coupled variable is used to set the lower bound.

!alert warning
Irreversibility must be used on a nodal auxiliary variable!

!syntax parameters /AuxKernels/Irreversibility

!syntax inputs /AuxKernels/Irreversibility

!syntax children /AuxKernels/Irreversibility

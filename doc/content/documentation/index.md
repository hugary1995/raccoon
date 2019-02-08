# Modules

RACCOON builds upon MOOSE to encapsulate common kernels, boundary conditions, etc. to prevent
code duplication.

Current version of RACCOON supports Mechanical-Fracture coupling only.
Feel free to create an issue on Github to ask for additional modules support.

!syntax list /Modules objects=True actions=False subsystems=False

!syntax list /Modules objects=False actions=False subsystems=True

!syntax list /Modules objects=False actions=True subsystems=False

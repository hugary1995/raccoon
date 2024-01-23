# Modules

RACCOON builds upon MOOSE to encapsulate common kernels, boundary conditions, etc. to prevent
code duplication.

Current version of RACCOON supports Thermal-Mechanical-Fracture coupling only. Therefore dependent MOOSE modules include:

- [Tensor Mechanics](modules/tensor_mechanics/index.md)
- [Heat Conduction](modules/heat_transfer/index.md)
- [Phase Field](modules/phase_field/index.md)

Feel free to create an issue on Github to ask for additional modules support.

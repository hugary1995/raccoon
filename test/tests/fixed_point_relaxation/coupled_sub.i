# Sub-app of coupled_main.i. All-Neumann diffusion + reaction gives the uniform relation
# d = 0.5*u, closing the two-way coupling.

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 2
    ny = 2
  []
[]

[Variables]
  [d]
  []
[]

[AuxVariables]
  [u]
  []
[]

[Kernels]
  [diff]
    type = Diffusion
    variable = d
  []
  [reaction]
    type = Reaction
    variable = d
  []
  [from_u]
    type = CoupledForce
    variable = d
    v = u
    coef = 0.5
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  line_search = none
  num_steps = 1
  dt = 1
[]

[Outputs]
[]

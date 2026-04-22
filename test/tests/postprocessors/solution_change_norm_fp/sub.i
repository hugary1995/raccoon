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

[Kernels]
  [diff]
    type = Diffusion
    variable = d
  []
[]

[BCs]
  [all]
    type = FunctionDirichletBC
    variable = d
    boundary = 'left right top bottom'
    function = 't'
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  num_steps = 2
  dt = 1
[]

[Outputs]
[]

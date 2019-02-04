[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Variables]
  [./d]
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = d
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = d
    boundary = left
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = d
    boundary = right
    value = 1
  [../]
[]

[Materials]
  [./local_dissipation]
    type = LocalDissipation
    f_name = 'w_d'
    args = 'd'
    function = 'd^2'
    outputs = exodus
  [../]
[]

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]

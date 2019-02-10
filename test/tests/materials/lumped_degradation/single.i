[GlobalParams]
  outputs = exodus
[]

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
  [./quadratic]
    type = DerivativeParsedMaterial
    f_name = 'g_d'
    args = 'd'
    constant_names = 'eta'
    constant_expressions = '0.1'
    function = '(1-eta)*(1-d)^2+eta'
    derivative_order = 2
  [../]
  [./lump]
    type = LumpedDegradation
    damage_fields = d
    degradation_name = g
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

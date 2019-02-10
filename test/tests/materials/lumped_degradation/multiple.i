[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Variables]
  [./d1]
  [../]
  [./d2]
  [../]
[]

[Kernels]
  [./diff1]
    type = Diffusion
    variable = d1
  [../]
  [./diff2]
    type = Diffusion
    variable = d2
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = d1
    boundary = left
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = d1
    boundary = right
    value = 1
  [../]
  [./top]
    type = DirichletBC
    variable = d2
    boundary = top
    value = 0
  [../]
  [./bottom]
    type = DirichletBC
    variable = d2
    boundary = bottom
    value = 1
  [../]
[]

[Materials]
  [./quadratic]
    type = DerivativeParsedMaterial
    f_name = 'g_d1'
    args = 'd1'
    function = '(1-d1)^2'
    derivative_order = 2
  [../]
  [./cubic]
    type = DerivativeParsedMaterial
    f_name = 'g_d2'
    args = 'd2'
    function = '(1-d2)^3'
    derivative_order = 2
  [../]
  [./lump]
    type = LumpedDegradation
    damage_fields = 'd1 d2'
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

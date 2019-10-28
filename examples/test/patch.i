[Mesh]
  type = FileMesh
  file = 'gold/geo.msh'
[]

[Functions]
  [./sln]
    type = ParsedFunction
    value = '3*x-9*y+7'
  [../]
[]

[Variables]
  [./T]
  [../]
[]

[Kernels]
  [./diff]
    type = MatDiffusion
    variable = T
    diffusivity = kappa
  [../]
[]

[BCs]
  [./all]
    type = FunctionPresetBC
    variable = T
    function = sln
    boundary = 'left right top bottom'
  [../]
[]

[Materials]
  [./diffusivity]
    type = GenericConstantMaterial
    prop_names = 'kappa'
    prop_values = '2.5'
  [../]
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
[]

[Postprocessors]
  [./read]
    type = NodalVariableValue
    nodeid = 2
    variable = T
  [../]
[]

[Outputs]
  exodus = true
[]

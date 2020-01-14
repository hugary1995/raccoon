[Mesh]
  type = GeneratedMesh
  dim = 2
  elem_type = QUAD4
[]

[Functions]
  [./solution]
    type = ParsedFunction
    value = 'exp(2*x)'
  [../]
[]

[Variables]
  [./u]
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = 'u'
  [../]
  [./react]
    type = MaterialPropertyUserObjectReaction
    variable = 'u'
    uo_name = 'c'
  [../]
[]

[BCs]
  [./left]
    type = FunctionDirichletBC
    variable = 'u'
    boundary = 'left'
    function = 'solution'
  [../]
  [./right]
    type = FunctionDirichletBC
    variable = 'u'
    boundary = 'right'
    function = 'solution'
  [../]
[]

[Materials]
  [./c]
    type = GenericConstantMaterial
    prop_names = 'c'
    prop_values = '4'
  [../]
[]

[UserObjects]
  [./c]
    type = MaterialPropertyUserObject
    mat_prop = 'c'
    execute_on = 'INITIAL LINEAR NONLINEAR'
  [../]
[]

[Postprocessors]
  [./l2_error]
    type = ElementL2Error
    variable = 'u'
    function = 'solution'
  [../]
[]

[Preconditioning]
  [./smp]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Steady
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
[]

[Outputs]
  exodus = true
[]

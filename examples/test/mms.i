[Mesh]
  type = FileMesh
  file = 'gold/geo.msh'
  uniform_refine = 0
[]

[Functions]
  [./sln]
    type = ParsedFunction
    value = 'exp(x+y)/(x^2+y+3)'
  [../]
  [./source]
    type = ParsedFunction
    value = '(2*exp(x + y)*(-2*x^3 + x^4 + (2 + y)^2 - 2*x*(3 + y) + 2*x^2*(4 + y)))/(3 + x^2 + y)^3'
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
  [./source]
    type = BodyForce
    variable = T
    function = source
    value = -1
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
    prop_values = '1'
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
    type = ElementL2Error
    variable = T
    function = sln
  [../]
[]

[Outputs]
  exodus = true
[]

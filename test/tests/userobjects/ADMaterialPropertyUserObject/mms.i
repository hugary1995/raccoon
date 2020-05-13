# This input file defines a quasi-1D equation:
# - u,xx + 4 * u = 0
# u(0) = 1, u(1) = e ^ 2
# u = e ^ (2 * x)
# Each step the mesh is uniformly refined once.
# The manufactured solution should have a convergence order of 2.

[Mesh]
  type = GeneratedMesh
  dim = 2
[]

[Adaptivity]
  marker = 'box'
  [./Markers]
    [./box]
      type = BoxMarker
      bottom_left = '0 0 0'
      top_right = '1 1 0 '
      inside = 'refine'
      outside = 'do_nothing'
    [../]
  [../]
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
    type = ADMaterialPropertyUserObjectReaction
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
    type = ADGenericConstantMaterial
    prop_names = 'c'
    prop_values = '4'
  [../]
[]

[UserObjects]
  [./c]
    type = ADMaterialPropertyUserObject
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

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  num_steps = 5
[]

[Outputs]
  exodus = true
[]

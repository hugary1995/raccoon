[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = -1
  xmax = 1
  nx = 200
[]

[Functions]
  [./forcing_fn]
    type = ParsedFunction
    value = '2*t*x'
  [../]
  [./exact_fn]
    type = ParsedFunction
    value = 't^2*x'
  [../]
  [./accel_fn]
    type = ParsedFunction
    value = '2*x'
  [../]
[]

[Variables]
  [./u]
  [../]
[]

[AuxVariables]
  [./accel]
  [../]
[]

[AuxKernels]
  [./accel]
    type = ExplicitAccelAux
    variable = 'accel'
    displacement = 'u'
  [../]
[]

[Kernels]
  [./ie]
    type = TimeDerivative
    variable = 'u'
  [../]
  [./diff]
    type = Diffusion
    variable = 'u'
  [../]
  [./ffn]
    type = BodyForce
    variable = 'u'
    function = 'forcing_fn'
  [../]
[]

[BCs]
  [./all]
    type = FunctionDirichletBC
    variable = 'u'
    boundary = '0 1'
    function = 'exact_fn'
  [../]
[]

[Postprocessors]
  [./l2_err_u]
    type = ElementL2Error
    variable = 'u'
    function = 'exact_fn'
  [../]
  [./l2_err_accel]
    type = ElementL2Error
    variable = 'accel'
    function = 'accel_fn'
  [../]
[]

[Executioner]
  type = Transient

  start_time = 0.0
  num_steps = 20
  dt = 0.00005
[]

[Outputs]
  exodus = true
[]

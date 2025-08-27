[Problem]
  solve = false
[]

[Mesh]
  [ori]
    type = GeneratedMeshGenerator
    dim = 2
    xmin = 0
    xmax = 100
    ymin = 0
    ymax = 100
    nx = '${fparse floor(100/max(1,pow(2,patchref)))}'
    ny = '${fparse floor(100/max(1,pow(2,patchref)))}'
  []
[]

[AuxVariables]
  [scale]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[ICs]
  [raw]
    type = RandomIC
    variable = scale
    max = 1
    min = -2
    seed = ${seed}
  []
[]

[Materials]
  [sigma_ts]
    type = ADParsedMaterial
    property_name = 'sigma_ts'
    constant_names = 'sigma_ts_raw'
    constant_expressions = '${sigma_ts}'
    coupled_variables = 'scale'
    expression = 'sigma_ts_raw*(10+ceil(scale))/10'
    output_properties = 'sigma_ts'
    outputs = exodus
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package '
  petsc_options_value = 'lu      superlu_dist    '
  automatic_scaling = true

  line_search = bt
  compute_scaling_once = true

  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10

  start_time = 0
  num_steps = 1
[]

[Outputs]
  [exodus]
    type = Exodus
  []
[]

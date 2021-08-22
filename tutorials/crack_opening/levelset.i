[MultiApps]
  [cod_1]
    type = FullSolveMultiApp
    input_files = cod_1.i
    cli_args = 'N=${N};a=${a}'
    execute_on = 'TIMESTEP_END'
  []
  [cod_2]
    type = FullSolveMultiApp
    input_files = cod_2.i
    cli_args = 'N=${N};a=${a}'
    execute_on = 'TIMESTEP_END'
  []
  [cod_3]
    type = FullSolveMultiApp
    input_files = cod_3.i
    cli_args = 'N=${N};a=${a}'
    execute_on = 'TIMESTEP_END'
  []
  [cod_4]
    type = FullSolveMultiApp
    input_files = cod_4.i
    cli_args = 'N=${N};a=${a}'
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [to_cod_1]
    type = MultiAppCopyTransfer
    multi_app = cod_1
    direction = to_multiapp
    variable = 'd disp_x disp_y phi cti'
    source_variable = 'd disp_x disp_y phi cti'
  []
  [to_cod_2]
    type = MultiAppCopyTransfer
    multi_app = cod_2
    direction = to_multiapp
    variable = 'd disp_x disp_y phi cti'
    source_variable = 'd disp_x disp_y phi cti'
  []
  [to_cod_3]
    type = MultiAppCopyTransfer
    multi_app = cod_3
    direction = to_multiapp
    variable = 'd disp_x disp_y phi cti'
    source_variable = 'd disp_x disp_y phi cti'
  []
  [to_cod_4]
    type = MultiAppCopyTransfer
    multi_app = cod_4
    direction = to_multiapp
    variable = 'd disp_x disp_y phi cti'
    source_variable = 'd disp_x disp_y phi cti'
  []
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = ${N}
    ny = ${N}
  []
[]

[Variables]
  [phi]
  []
  [cti]
  []
[]

[AuxVariables]
  [d]
  []
  [disp_x]
  []
  [disp_y]
  []
[]

[Kernels]
  [diff]
    type = RankTwoTensorAnisoDiffusion
    variable = phi
    diffusivity = D2
  []
  [f]
    type = ADCoefMatSource
    variable = phi
    prop_names = f
  []
  [penalty]
    type = ADCoefMatSource
    variable = phi
    prop_names = penalty
  []
  [diff_cti]
    type = RankTwoTensorAnisoDiffusion
    variable = cti
    diffusivity = D
  []
  [react]
    type = Reaction
    variable = cti
  []
  [source]
    type = ADCoefMatSource
    variable = cti
    prop_names = source
    coefficient = -1
  []
[]

[Materials]
  [f]
    type = ADParsedMaterial
    f_name = f
    args = d
    function = 'if(d>0,-20,10)'
  []
  [penalty]
    type = ADParsedMaterial
    f_name = penalty
    args = 'phi d'
    constant_names = 'p dls'
    constant_expressions = '1e6 1e-2'
    function = 'if(d<dls & d>0, 2*p*phi, 0)'
  []
  [D]
    type = Rectifier
    phase_field = d
    small_D = 1e-4
    large_D = 1
  []
  [D2]
    type = Rectifier
    phase_field = d
    small_D = 1e-4
    large_D = 1
    diffusivity = D2
  []
  [source]
    type = ADParsedMaterial
    f_name = source
    args = d
    function = 'd'
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist                 '
  automatic_scaling = true

  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10

  num_steps = 1
[]

[Outputs]
  print_linear_residuals = false
[]

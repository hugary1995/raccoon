[MultiApps]
  [cod]
    type = FullSolveMultiApp
    input_files = cod.i
    cli_args = 'refine=${refine};h=${h};l_over_h=${l_over_h}'
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [to_cod]
    type = MultiAppMeshFunctionTransfer
    to_multi_app = cod
    variable = 'd disp_x disp_y T phi'
    source_variable = 'd disp_x disp_y T phi'
  []
  [to_cod_cti]
    type = MultiAppProjectionTransfer
    to_multi_app = cod
    variable = 'cti'
    source_variable = 'cti'
  []
[]

[Problem]
  kernel_coverage_check = false
  material_coverage_check = false
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 10
    ny = 10
  []
  [mid]
    type = SubdomainBoundingBoxGenerator
    input = gen
    block_id = 1
    bottom_left = '0 0.45 0'
    top_right = '1 0.55 0'
  []
  [refine]
    type = RefineBlockGenerator
    input = mid
    block = 1
    refinement = ${refine}
  []
[]

[Variables]
  [phi]
    block = 1
  []
  [cti]
    block = 1
  []
[]

[AuxVariables]
  [d]
  []
  [disp_x]
  []
  [disp_y]
  []
  [T]
  []
[]

[UserObjects]
  [active_domain]
    type = CoupledVarThresholdElementSubdomainModifier
    apply_initial_conditions = false
    coupled_var = d
    criterion_type = ABOVE
    threshold = 1e-10
    subdomain_id = 1
    complement_subdomain_id = 0
    execute_on = TIMESTEP_BEGIN
  []
[]

[Kernels]
  [levelset_diff]
    type = ADDiffusion
    variable = phi
    block = 1
  []
  [f]
    type = ADCoefMatSource
    variable = phi
    prop_names = f
    block = 1
  []
  [penalty]
    type = ADCoefMatSource
    variable = phi
    prop_names = penalty
    block = 1
  []

  [diff_cti]
    type = RankTwoTensorAnisoDiffusion
    variable = cti
    diffusivity = D
    block = 1
  []
  [react]
    type = Reaction
    variable = cti
    block = 1
  []
  [source]
    type = ADCoefMatSource
    variable = cti
    prop_names = source
    coefficient = -1
    block = 1
  []
[]

[Materials]
  [f]
    type = ADParsedMaterial
    f_name = f
    args = d
    function = 'if(d>0,-20,10)'
    block = 1
  []
  [levelset_penalty]
    type = ADParsedMaterial
    f_name = penalty
    args = 'phi d'
    constant_names = 'p'
    constant_expressions = '1e6'
    function = 'if(d<${dls} & d>0, 2*p*phi, 0)'
    block = 1
  []
  [D]
    type = Rectifier
    phase_field = d
    small_D = 1e-4
    large_D = 1
    block = 1
  []
  [source]
    type = ADParsedMaterial
    f_name = source
    args = d
    function = 'd'
    block = 1
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

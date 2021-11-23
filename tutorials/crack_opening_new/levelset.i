[MultiApps]
  [cod]
    type = FullSolveMultiApp
    input_files = cod.i
    cli_args = 'e=${e};a=${a}'
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [to_cod]
    type = MultiAppMeshFunctionTransfer
    multi_app = cod
    direction = to_multiapp
    variable = 'd disp_x disp_y phi'
    source_variable = 'd disp_x disp_y phi'
  []
[]

[Problem]
  kernel_coverage_check = false
  material_coverage_check = false
[]

[Mesh]
  [gen]
    type = FileMeshGenerator
    file = 'gold/plate_${e}.msh'
  []
  [block0]
    type = SubdomainBoundingBoxGenerator
    input = gen
    bottom_left = '0 0.5 0'
    top_right = '1 1 0'
    block_id = 0
  []
  [block1]
    type = SubdomainBoundingBoxGenerator
    input = block0
    bottom_left = '0 0 0'
    top_right = '1 0.5 0'
    block_id = 1
  []
[]

[Variables]
  [phi]
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
[]

[UserObjects]
  [active_domain]
    type = CoupledVarThresholdElementSubdomainModifier
    apply_initial_conditions = false
    coupled_var = d
    criterion_type = ABOVE
    threshold = 0
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
    constant_names = 'p dls'
    constant_expressions = '1e6 1e-2'
    function = 'if(d<dls & d>0, 2*p*phi, 0)'
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

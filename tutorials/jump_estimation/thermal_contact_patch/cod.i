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
  [w]
    block = 1
  []
  [wT_x]
    block = 1
  []
  [wT_y]
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
  [cod]
    order = CONSTANT
    family = MONOMIAL
    [AuxKernel]
      type = ADMaterialRealAux
      property = crack_opening_displacement
      block = 1
    []
  []
  [phi]
  []
  [cti]
  []
[]

[UserObjects]
  [active_domain]
    type = CoupledVarThresholdElementSubdomainModifier
    apply_initial_conditions = false
    coupled_var = cti
    criterion_type = ABOVE
    threshold = 0.3
    subdomain_id = 1
    complement_subdomain_id = 0
    execute_on = TIMESTEP_BEGIN
  []
[]

[Kernels]
  [cod]
    type = Nitsche
    variable = w
    level_set = phi
    alpha = alpha
    support = '${fparse 4*l}'
    block = 1
  []
  [T_jump_x]
    type = JumpEstimator
    variable = wT_x
    level_set = phi
    alpha = alpha_T
    jump = T_jump
    component = 0
    support = '${fparse 4*l}'
    block = 1
  []
  [T_jump_y]
    type = JumpEstimator
    variable = wT_y
    level_set = phi
    alpha = alpha_T
    jump = T_jump
    component = 1
    support = '${fparse 4*l}'
    block = 1
  []
[]

[Materials]
  [cod]
    type = CrackOpeningDisplacement
    phase_field = d
    level_set = d
    displacements = 'disp_x disp_y'
    block = 1
  []
  [T_jump]
    type = VariableJump
    variable = T
    jump = T_jump
    phase_field = d
    level_set = d
  []
  [penalty]
    type = ADParsedMaterial
    f_name = alpha
    args = 'd'
    function = '1*d'
    block = 1
  []
  [penalty_T]
    type = ADParsedMaterial
    f_name = alpha_T
    args = 'd'
    function = '1e2*d'
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
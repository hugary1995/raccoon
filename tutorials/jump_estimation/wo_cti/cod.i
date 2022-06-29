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
[]

[AuxVariables]
  [d]
  []
  [disp_x]
  []
  [disp_y]
  []
  [cod]
    order = CONSTANT
    family = MONOMIAL
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

[AuxKernels]
  [cod]
    type = ADMaterialRealAux
    variable = cod
    property = crack_opening_displacement
    block = 1
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
[]

[Materials]
  [cod]
    type = CrackOpeningDisplacement
    phase_field = d
    level_set = d
    displacements = 'disp_x disp_y'
    block = 1
  []
  [penalty]
    type = ADParsedMaterial
    f_name = alpha
    args = 'd'
    function = '1*d'
    outputs = exodus
    block = 1
  []
[]

[VectorPostprocessors]
  [w]
    type = LineValueSampler
    variable = w
    start_point = '0 0.5 0'
    end_point = '1 0.5 0'
    num_points = 1000
    sort_by = x
  []
  [uy+]
    type = LineValueSampler
    variable = disp_y
    start_point = '0 ${fparse 0.5+2.5*h} 0'
    end_point = '1 ${fparse 0.5+2.5*h} 0'
    num_points = 1000
    sort_by = x
  []
  [uy-]
    type = LineValueSampler
    variable = disp_y
    start_point = '0 ${fparse 0.5-2.5*h} 0'
    end_point = '1 ${fparse 0.5-2.5*h} 0'
    num_points = 1000
    sort_by = x
  []
  [w_center]
    type = LineValueSampler
    variable = w
    start_point = '0.5 0.4 0'
    end_point = '0.5 0.6 0'
    num_points = 100
    sort_by = y
  []
  [d_center]
    type = LineValueSampler
    variable = d
    start_point = '0.5 0.4 0'
    end_point = '0.5 0.6 0'
    num_points = 100
    sort_by = y
  []
  [uy_center]
    type = LineValueSampler
    variable = disp_y
    start_point = '0.5 0.4 0'
    end_point = '0.5 0.6 0'
    num_points = 100
    sort_by = y
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
  [exodus]
    type = Exodus
    file_base = 'output/good_a_${a}'
  []
  [csv]
    type = CSV
    file_base = 'data/good_a_${a}'
    execute_on = 'TIMESTEP_END'
  []
[]

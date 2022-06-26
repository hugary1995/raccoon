[Problem]
  kernel_coverage_check = false
  material_coverage_check = false
[]

[Mesh]
  [gen]
    type = FileMeshGenerator
    file = 'gold/plate_${h}.msh'
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
    threshold = 0.27
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
  [diff]
    type = ADCoefMatDiffusion
    variable = w
    coefficient = 0
    block = 1
  []
  [cod]
    type = Nitsche
    variable = w
    level_set = phi
    alpha = alpha
    block = 1
  []
[]

[Materials]
  [cod]
    type = CrackOpeningDisplacement
    phase_field = d
    level_set = phi
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
    start_point = '0 ${fparse 0.5+2*h} 0'
    end_point = '1 ${fparse 0.5+2*h} 0'
    num_points = 1000
    sort_by = x
  []
  [uy-]
    type = LineValueSampler
    variable = disp_y
    start_point = '0 ${fparse 0.5-2*h} 0'
    end_point = '1 ${fparse 0.5-2*h} 0'
    num_points = 1000
    sort_by = x
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
    file_base = 'output/h_${h}'
  []
  [csv]
    type = CSV
    file_base = 'data/h_${h}'
    execute_on = 'TIMESTEP_END'
  []
[]

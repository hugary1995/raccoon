[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = ${N}
    ny = ${N}
  []
[]

[Variables]
  [w]
  []
[]

[AuxVariables]
  [d]
  []
  [disp_x]
  []
  [disp_y]
  []
  [phi]
  []
  [cod]
    order = CONSTANT
    family = MONOMIAL
  []
  [cti]
  []
[]

[AuxKernels]
  [cod]
    type = ADMaterialRealAux
    variable = cod
    property = crack_opening_displacement
  []
[]

[Kernels]
  [diff]
    type = ADCoefMatDiffusion
    variable = w
    coefficient = 0
  []
  [cod]
    type = Nitsche
    variable = w
    levelset = phi
    phase_field = d
    alpha = 1e3
    cti = cti
    cti_threshold = 0.75
    cti_max = cti_max
    normal = '0 1 0'
  []
[]

[Materials]
  [cod]
    type = CrackOpeningDisplacement
    phase_field = d
    levelset = phi
    displacements = 'disp_x disp_y'
    normal = '0 1 0'
  []
[]

[Postprocessors]
  [cti_max]
    type = NodalExtremeValue
    variable = cti
    value_type = max
    execute_on = 'TIMESTEP_BEGIN'
    outputs = none
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
    file_base = 'output/correct_normal_wo_crack_tip_a_${a}'
  []
  [csv]
    type = CSV
    file_base = 'data/correct_normal_wo_crack_tip_a_${a}'
    execute_on = 'TIMESTEP_END'
  []
[]

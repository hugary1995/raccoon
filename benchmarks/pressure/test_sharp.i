[Mesh]
  type = FileMesh
  file = bubble.e
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Problem]
  type = FixedPointProblem
[]

[UserObjects]
  [E_driving]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'E_el_active'
  []
  [pressure_uo]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'p'
  []
[]

[Variables]
  [d]
  []
  [disp_x]
  []
  [disp_y]
  []
[]

[Modules]
  [TensorMechanics]
    [Master]
      [mech]
        add_variables = true
        strain = SMALL
        additional_generate_output = 'stress_yy stress_xy stress_xx strain_xx strain_xy strain_yy '
                                     'strain_zz hydrostatic_stress mid_principal_stress '
                                     'min_principal_stress max_principal_stress'
        save_in = 'force_x force_y'
        use_automatic_differentiation = true
      []
    []
  []
[]

[AuxVariables]
  [force_x]
  []
  [force_y]
  []
  [C1111]
    order = CONSTANT
    family = MONOMIAL
  []
  [bounds_dummy]
  []
[]

[Functions]
  [pressure]
    type = PiecewiseLinear
    x = '0 10'
    y = '0 200'
  []
[]

[Bounds]
  [d_upper_bound]
    type = ConstantBoundsAux
    variable = bounds_dummy
    bounded_variable = d
    bound_type = upper
    bound_value = 1.0
  []
  [d_lower_bound]
    type = VariableOldValueBoundsAux
    variable = bounds_dummy
    bounded_variable = d
    bound_type = lower
  []
[]

[Kernels]
  [pressure_body_force_x]
    type = ADPressurizedCrack
    variable = 'disp_x'
    d = 'd'
    pressure_mat = 'p'
    component = 0
    xi = 0
  []
  [pressure_body_force_y]
    type = ADPressurizedCrack
    variable = 'disp_y'
    d = 'd'
    pressure_mat = 'p'
    component = 1
    xi = 0
  []
  [pff_diff]
    type = ADPFFDiffusion
    variable = 'd'
  []
  [pff_barrier]
    type = ADPFFBarrier
    variable = 'd'
  []
  [pff_react]
    type = ADPFFReaction
    variable = 'd'
    driving_energy_uo = 'E_driving'
    lag = false
  []
  [pff_pressure]
    type = ADPFFPressure
    variable = 'd'
    pressure_uo = 'pressure_uo'
    displacements = 'disp_x disp_y'
    xi = 0
  []
[]

[AuxKernels]
  [C1111]
    type = ADRankFourAux
    variable = C1111
    rank_four_tensor = elasticity_tensor
    index_l = 0
    index_j = 0
    index_k = 0
    index_i = 0
    execute_on = timestep_end
  []
[]

[Materials]
  [pressure]
    type = ADGenericFunctionMaterial
    prop_names = 'p'
    prop_values = 'pressure'
  []
  [elasticity_tensor]
    type = ADComputeElasticityTensor
    C_ijkl = '385000 0.23'
    fill_method = symmetric_isotropic_E_nu
  []
  [stress]
    type = SmallStrainDegradedElasticPK2Stress_StrainVolDev
    d = 'd'
    outputs = 'exodus'
    output_properties = 'E_el_active'
  []
  [bulk]
    type = GenericConstantMaterial
    prop_names = 'phase_field_regularization_length energy_release_rate critical_fracture_energy'
    prop_values = '0.2 0.001 0.021948052'
  []
  [local_dissipation]
    type = QuadraticLocalDissipation
    d = 'd'
    xi = 2
  []
  [fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 'pi'
  []
  [degradation]
    type = WuDegradation
    d = 'd'
    a2 = -0.5
    a3 = 0
    xi = 2
  []
[]

[BCs]
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = '1'
    value = 0
    preset = false
  []
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = '2'
    value = 0
    preset = false
  []
  [Pressure]
    [bubble]
      boundary = 5
      function = pressure
      displacements = 'disp_x disp_y'
    []
  []
[]

# [Postprocessors]
#   [ave_stress_top]
#     type = SideAverageValue
#     variable = stress_yy
#     boundary = 4
#   []
#   [disp_y_top]
#     type = SideAverageValue
#     variable = disp_y
#     boundary = 4
#   []
#   [react_y_top]
#     type = NodalSum
#     variable = force_y
#     boundary = 4
#   []
# []

[Executioner]
  type = FixedPointTransient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_type -snes_type'
  petsc_options_value = 'lu superlu_dist vinewtonrsls'
  nl_rel_tol = 1e-8 ##nonlinear relative tolerance
  nl_abs_tol = 1e-8
  l_max_its = 10 ##max linear iterations Previous:200
  nl_max_its = 50 ##max nonlinear iterations Previous:50
  start_time = 0
  line_search = 'none'
  end_time = 10
  dt = 0.5
  automatic_scaling = true
  fp_max_its = 100
  fp_tol = 1e-06
  accept_on_max_fp_iteration = true
  # [TimeStepper]
  #   type = IterationAdaptiveDT
  #   dt = 1e-1
  #   optimal_iterations = 10
  #   iteration_window = 0
  #   growth_factor = 1.2
  #   cutback_factor = 0.5
  # []
[]

[Outputs]
  print_linear_residuals = false
  [exodus]
    type = Exodus
    interval = 1
  []
[]

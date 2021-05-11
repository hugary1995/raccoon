[MultiApps]
  [mechanical]
    type = TransientMultiApp
    input_files = 'mechanical.i'
    app_type = raccoonApp
  []
[]

[Transfers]
  [get_we_active]
    type = MultiAppMeshFunctionTransfer
    multi_app = 'mechanical'
    direction = from_multiapp
    source_variable = 'we_active'
    variable = 'we_active'
  []
  [send_d]
    type = MultiAppMeshFunctionTransfer
    multi_app = 'mechanical'
    direction = to_multiapp
    source_variable = d
    variable = d
  []
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = 'gold/fields.e'
    use_for_exodus_restart = true
  []
[]

[Variables]
  [d]
  []
[]

[AuxVariables]
  [we_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [bounds_dummy]
  []
  [Gc]
    initial_from_file_var = 'Gc'
  []
  [psic]
    initial_from_file_var = 'psic'
  []
[]

[Bounds]
  [irreversibility]
    type = VariableOldValueBoundsAux
    variable = bounds_dummy
    bounded_variable = d
    bound_type = lower
  []
  [upper]
    type = ConstantBoundsAux
    variable = bounds_dummy
    bounded_variable = d
    bound_type = upper
    bound_value = 1
  []
[]

[Kernels]
  [pff_diff]
    type = ADPFFDiffusion
    variable = d
    mobility_name = L
    kappa_name = kappa
  []
  [pff_react]
    type = ADPFFSource
    variable = d
    free_energy = psi
  []
[]

[Materials]
  [energy_release_rate]
    type = ADParsedMaterial
    f_name = 'Gc'
    args = 'Gc'
    function = 'Gc'
  []
  [critial_fracture_energy]
    type = ADParsedMaterial
    f_name = 'psic'
    args = 'psic'
    function = 'psic'
  []
  [length_scale]
    type = ADGenericConstantMaterial
    prop_names = 'l'
    prop_values = '1'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    d = d
  []
  [degradation]
    type = RationalDegradationFunction
    f_name = g
    d = d
    parameter_names = 'p a2 a3 eta '
    parameter_values = '2 1 0 1e-6'
    material_property_names = 'Gc psic xi c0 l '
  []
  [active_elastic_energy]
    type = ADParsedMaterial
    f_name = we_active
    function = 'we_active'
    args = we_active
  []
  [psi]
    type = ADDerivativeParsedMaterial
    f_name = psi
    function = 'alpha/c0/l+g*we_active/Gc'
    args = d
    material_property_names = 'alpha(d) c0 l g(d) we_active Gc'
    derivative_order = 1
  []
[]

[BCs]
  [Periodic]
    [all]
      variable = d
      auto_direction = 'x y'
    []
  []
[]

[Postprocessors]
  [d_norm]
    type = NodalSum
    variable = d
    outputs = none
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'
  dt = 1e-3
  end_time = 0.22

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  picard_max_its = 10
  disable_picard_residual_norm_check = true
  picard_custom_pp = d_norm
  custom_abs_tol = 1e-3
  custom_rel_tol = 1e-3
  accept_on_max_picard_iteration = true
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

[MultiApps]
  [mechanical]
    type = TransientMultiApp
    input_files = 'mechanical.i'
    app_type = raccoonApp
  []
[]

[Transfers]
  [get_E_el_active]
    type = MultiAppMeshFunctionTransfer
    multi_app = 'mechanical'
    direction = from_multiapp
    source_variable = 'E_el_active'
    variable = 'E_el_active'
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
  [E_el_active]
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
  [pff_barr]
    type = ADPFFBarrier
    variable = d
    mobility_name = L
    local_dissipation_name = alpha
  []
  [pff_react]
    type = ADPFFReaction
    variable = d
    driving_energy_var = 'E_el_active'
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
  [mobility]
    type = ADParsedMaterial
    f_name = L
    material_property_names = 'Gc c0 l'
    function = 'Gc/c0/l'
  []
  [kappa]
    type = ADParsedMaterial
    f_name = kappa
    material_property_names = 'l'
    function = '2*l^2'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    d = d
    initial_derivative_name = xi
    normalization_constant_name = c0
  []
  [degradation]
    type = RationalDegradationFunction
    f_name = g
    d = d
    parameter_names = 'p a2 a3 eta '
    parameter_values = '2 1 0 1e-6'
    material_property_names = 'Gc psic xi c0 l '
  []
[]

# There is an issue with periodic BC due to ghosting functors.
# This issue should be fixed pretty soon.
# [BCs]
#   [Periodic]
#     [all]
#       variable = u
#       auto_direction = 'x y'
#     []
#   []
# []

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

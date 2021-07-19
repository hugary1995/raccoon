[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 1
    ny = 1
    nz = 1
  []
[]


[Variables]
  [d]
  []
[]

[AuxVariables]
  [bounds_dummy]
  []
  [stress_00]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_01]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_02]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_11]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_12]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_22]
    order = CONSTANT
    family = MONOMIAL
  []
  [psie_active]
    order = CONSTANT
    family = MONOMIAL
  []
  # [ce]
  #   order = CONSTANT
  #   family = MONOMIAL
  # []
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
  [diff]
    type = ADPFFDiffusion
    variable = d
    fracture_toughness = Gc
    regularization_length = l
    normalization_constant = c0
  []
  [source]
    type = ADPFFSource
    variable = d
    free_energy = psi
  []
[]

[Materials]
  [fracture_properties]
    type = ADGenericConstantMaterial
    prop_names = 'Gc l Lambda G'
    prop_values = '${Gc} ${l} ${Lambda} ${G}'
  []
  [degradation]
    type = PowerDegradationFunction
    f_name = g
    function = (1-d)^p*(1-eta)+eta
    phase_field = d
    parameter_names = 'p eta '
    parameter_values = '2 0'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    phase_field = d
  []
  [psi]
    type = ADDerivativeParsedMaterial
    f_name = psi
    function = '2*d/c0*(4.0/3.0*d*psie_active-8.0/3.0*psie_active+4.0/3.0*ce+Gc/2.0/l)'
    args = 'd  psie_active'
    material_property_names = 'Gc c0 l ce  ' #alpha(d) g(d)
    derivative_order = 1
    # output_properties = dpsi
    # outputs = exodus
  []
  [kumar_material]
    type = GeneralizedExternalDrivingForce
    # first_invariant = invar_1
    # second_invariant = invar_2
    # rank_two_tensor = 'stress'
      stress_00 = stress_00
      stress_01 = stress_01
      stress_02 = stress_02
      stress_11 = stress_11
      stress_12 = stress_12
      stress_22 = stress_22
    tensile_strength = '${sigma_ts}' #27MPa
    compressive_strength = '${sigma_cs}' #77MPa
    delta = '${delta}'
    energy_release_rate = '${Gc}'
    phase_field_regularization_length = '${l}'
    Lame_first_parameter = '${Lambda}'
    shear_modulus = '${G}'
    external_driving_force_name = ce
    output_properties = 'F_surface J2 beta_0 beta_1 beta_2 beta_3 ce'
    outputs = exodus
  []
[]

[Postprocessors]
  [extdriving]
    type = ElementAverageValue
    variable = 'ce'
  []
  [d_avg]
    type = AverageNodalVariableValue
    variable = d
  []
[]

# [VectorPostprocessors]
#   [damage]
#     type = NodalValueSampler
#     variable = 'd'
#     sort_by = id
#   []
#   [ext]
#     type = ElementValueSampler
#     variable = 'ce'
#     sort_by = id
#   []
# []

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'
  automatic_scaling = true

  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10
[]

[Outputs]
  # [csv_]
  #   type = CSV
  #   file_base = kumar_1ele_t_Gc4L0.12del9.66_frac
  #   append_date = true
  #   execute_vector_postprocessors_on = final
  # []
  exodus = true
  file_base = kumar_1ele_c_Gc4L0.12del9.66_frac
  append_date = true
  print_linear_residuals = false
[]

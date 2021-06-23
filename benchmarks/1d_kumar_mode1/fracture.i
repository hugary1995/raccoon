# [Problem]
#   solve = false
# []

[Mesh]
  [gen]
  type = GeneratedMeshGenerator
  dim = 1
  nx = 20
  xmax = 1
  xmin = -1
  []
[]

[Variables]
  [d]
    [InitialCondition]
      type = FunctionIC
      function = 'if(x=0,0.5,0)'
    []
  []
[]

[AuxVariables]
  [bounds_dummy]
  []
  [psie_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [invar_1]
    order = CONSTANT
    family = MONOMIAL
  []
  [invar_2]
    order = CONSTANT
    family = MONOMIAL
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
    args = 'd psie_active'
    material_property_names = 'Gc c0 l ce' #alpha(d) g(d)
    derivative_order = 1
  []
  [kumar_material]
    type = GeneralizedExternalDrivingForce
    first_invariant = invar_1
    second_invariant = invar_2
    tensile_strength = '${sigma_ts}' #27MPa
    compressive_strength = '${sigma_cs}' #77MPa
    delta = '${delta}'
    energy_release_rate = '${Gc}'
    phase_field_regularization_length = '${l}'
    Lame_first_parameter = '${Lambda}'
    shear_modulus = '${G}'
    external_driving_force_name = ce
  []

[]

[Postprocessors]
  [extdriving]
    type = ADElementAverageMaterialProperty
    mat_prop = 'ce'
  []
  [beta_0]
    type = ADElementAverageMaterialProperty
    mat_prop = 'beta_0'
  []
  [beta_1]
    type = ADElementAverageMaterialProperty
    mat_prop = 'beta_1'
  []
  [beta_2]
    type = ADElementAverageMaterialProperty
    mat_prop = 'beta_2'
  []
  [beta_3]
    type = ADElementAverageMaterialProperty
    mat_prop = 'beta_3'
  []
  [F_surface]
    type = ADElementAverageMaterialProperty
    mat_prop = 'F_surface'
  []
  [d_avg]
    type = AverageNodalVariableValue
    variable = d
  []
[]


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
  [csv_]
    type = CSV
    file_base = 1d_kumar_frac_m20L0.05
    append_date = true
    #execute_vector_postprocessors_on = final
  []
  print_linear_residuals = false
[]

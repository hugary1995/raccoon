# [Problem]
#   solve = false
# []

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 10
    ny = 10
    nz = 10
  []
#   [noncrack]
#     type = BoundingBoxNodeSetGenerator
#     input = gen
#     new_boundary = noncrack
#     bottom_left = '0.5 0 0'
#     top_right = '1 0 0'
#   []
#   construct_side_list_from_node_list = true
[]

# [Adaptivity]
#   marker = marker
#   initial_marker = marker
#   initial_steps = 2
#   stop_time = 0
#   max_h_level = 2
#   [Markers]
#     [marker]
#       type = BoxMarker
#       bottom_left = '0.4 0 0'
#       top_right = '1 0.05 0'
#       outside = DO_NOTHING
#       inside = REFINE
#     []
#   []
# []

[Variables]
  [d]
  []
[]

[AuxVariables]
  [bounds_dummy]
  []
  [psie]
    order = CONSTANT
    family = MONOMIAL
  []
  [invar_1]
  []
  [invar_2]
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
    prop_names = 'Gc l'
    prop_values = '${Gc} ${l}'
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
    function = 'alpha/c0*(4.0/3.0*d*psie-8.0/3.0*psie+4.0/3.0*ce+Gc/2.0/l)'
    args = 'd psie'
    material_property_names = 'alpha(d) g(d) Gc c0 l ce'
    derivative_order = 1
  []
  [kumar_material]
    type = ExternalDrivingForce
    output_properties = delta
    first_invariant = invar_1
    second_invariant = invar_2
    tensile_strength = 27
    compressive_strength = 77
    delta = 1
    alpha_L = 0
    energy_release_rate = '${Gc}'
    phase_field_regularization_length = '${l}'
    external_driving_force_name = ce
  []

[]

[Postprocessors]
  [mat]
    type = ADElementAverageMaterialProperty
    mat_prop = 'ce'
  []
  # [invar_1]
  #   type = AverageNodalVariableValue
  #   variable = invar_1
  # []
  # [invar_2]
  #   type = AverageNodalVariableValue
  #   variable = invar_2
  # []
[]

[VectorPostprocessors]
  [damage]
    type = NodalValueSampler
    variable = 'd'
    sort_by = id
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
file_base = linear_frac
append_date = true
#show = 'var_u'
execute_vector_postprocessors_on = final
[]
  print_linear_residuals = false
[]

[Mesh]
  [top_half]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 252 #126
    ny = 28 #14
    nz = 1
    xmax = 126
    ymin = 0
    ymax = 14
    zmax = 0.5
    boundary_id_offset = 0
    boundary_name_prefix = top_half
  []
  [right_fixp]
    type = BoundingBoxNodeSetGenerator
    input = top_half
    new_boundary = right_fixp
    bottom_left = '119 0 0'
    top_right = '119 0 0.5'
  []
  [top_loadp]
    type = BoundingBoxNodeSetGenerator
    input = right_fixp
    new_boundary = top_loadp
    bottom_left = '72 14 0'
    top_right = '72 14 0.5'
  []
  [top_stitch]
    type = BoundingBoxNodeSetGenerator
    input = top_loadp
    new_boundary = top_stitch
    bottom_left = '0 0 0'
    top_right = '63 0 0.5'
  []
  [bottom_half]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 126 #63
    ny = 28 #14
    nz = 1
    xmax = 63
    ymin = -14
    ymax = 0
    zmax = 0.5
    boundary_id_offset = 10
    boundary_name_prefix = bottom_half
  []
  [left_fixp]
    type = BoundingBoxNodeSetGenerator
    input = bottom_half
    new_boundary = left_fixp
    bottom_left = '45 -14 0'
    top_right = '45 -14 0.5'
  []
  [bottom_stitch]
    type = BoundingBoxNodeSetGenerator
    input = left_fixp
    new_boundary = bottom_stitch
    bottom_left = '0 0 0'
    top_right = '63 0 0.5'
  []
  [stitch]
    type = StitchedMeshGenerator
    inputs = 'top_stitch bottom_stitch'
    stitch_boundaries_pairs = 'top_stitch bottom_stitch'
  []
  construct_side_list_from_node_list = true
[]

[Adaptivity]
  marker = marker
  initial_marker = marker
  initial_steps = 4
  stop_time = 0
  max_h_level = 4
  [Markers]
    [marker]
      type = BoxMarker
      bottom_left = '62.5 -1 0'
      top_right = '63.5 14 0.5'
      outside = DO_NOTHING
      inside = REFINE
    []
  []
[]


[Variables]
  [d]
  []
[]

[AuxVariables]
  [bounds_dummy]
  []
  [psie_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [ce]
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
    prop_names = 'Gc l' # Lambda G'
    prop_values = '${Gc} ${l}' #' ${Lambda} ${G}'
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
    args = 'd psie_active ce'
    material_property_names = 'Gc c0 l' #alpha(d) g(d)
    derivative_order = 1
  []
[]

[Postprocessors]
  # [extdriving]
  #   type = ElementAverageValue
  #   variable = 'ce'
  # []
  # [d_avg]
  #   type = AverageNodalVariableValue
  #   variable = d
  # []
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

# [Outputs]
#   # [csv_]
#   #   type = CSV
#   #   file_base = kumar_mode1_half_Gc4L0.35del4.41z1ele_frac
#   #   append_date = true
#   #   execute_vector_postprocessors_on = final
#   # []
#   exodus = true
#   file_base = corner_refine6_Gc1_t1.2dt5e-2_frac
#   append_date = true
#   print_linear_residuals = false
# []

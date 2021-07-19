[Mesh]
  [top_half]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 84 #750/8
    ny = 14 #125/8
    xmax = 30 #30
    ymin = 0
    ymax = 5 #5
    boundary_id_offset = 0
    boundary_name_prefix = top_half
  []
  [createNewSidesetOne]
    type = SideSetsFromBoundingBoxGenerator
    input = top_half
    block_id = 0
    boundary_id_old = 'top_half_bottom'
    boundary_id_new = 100
    bottom_left = '0 0 0'
    top_right = '5 0.36 0' #0.36
    # show_info = true
  []
  [top_stitch]
    type = BoundingBoxNodeSetGenerator
    input = createNewSidesetOne
    new_boundary = top_stitch
    bottom_left = '5 0 0' #5
    top_right = '30 0 0' #30
    # show_info = true
  []
  [bottom_half]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 84 #750/8
    ny = 14 #125/8
    xmax = 30 #0
    ymin = -5
    ymax = 0
    boundary_id_offset = 7
    boundary_name_prefix = bottom_half
  []
  # [bottom_nonstitch]
  #   type = BoundingBoxNodeSetGenerator
  #   input = bottom_half
  #   new_boundary = bottom_nonstitch
  #   bottom_left = '0 0 0'
  #   top_right = '5 0 0'
  #   show_info = true
  # []
  [createNewSidesetTwo]
    type = SideSetsFromBoundingBoxGenerator
    input = bottom_half
    block_id = 0
    boundary_id_old = 'bottom_half_top'
    boundary_id_new = 200
    bottom_left = '0 0 0'
    top_right = '5 -0.36 0'
    # show_info = true
  []
  [bottom_stitch]
    type = BoundingBoxNodeSetGenerator
    input = createNewSidesetTwo
    new_boundary = bottom_stitch
    bottom_left = '5 0 0'
    top_right = '30 0 0'
    # show_info = true
  []
  [stitch]
    type = StitchedMeshGenerator
    inputs = 'top_stitch bottom_stitch'
    stitch_boundaries_pairs = 'top_stitch bottom_stitch'
    # show_info = true
  []
  construct_side_list_from_node_list = true
[]

[Adaptivity]
  marker = marker
  initial_marker = marker
  initial_steps = 3
  stop_time = 0
  max_h_level = 3
  [Markers]
    [marker]
      type = BoxMarker
      bottom_left = '4.2 -0.8 0' #0.5
      top_right = '30 0.8 0'
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
  # [disp_x]
  # []
  # [disp_y]
  # []
[]


[Bounds]
  # [irreversibility]
  #   type = VariableOldValueBoundsAux
  #   variable = bounds_dummy
  #   bounded_variable = d
  #   bound_type = lower
  # []
  [lower]
    type = ConstantBoundsAux
    variable = bounds_dummy
    bounded_variable = d
    bound_type = lower
    bound_value = 0
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
  [cracksurface]
    type = CrackSurfaceDensity
    phase_field = d
    output_properties = gamma
    outputs = exodus
 []
[]

# [DomainIntegral]
#   crack_direction_method = CrackDirectionVector
#   incremental = true
#   integrals = JIntegral
#   2d = true
#   boundary = top_stitch
#   crack_direction_vector = '1 0 0'
#   radius_inner = '1 2.5'
#   radius_outer = '2.5 3.0'
#   # output_variable = 'disp_x'
#   # output_q = false
#   displacements = 'disp_x disp_y'
# []

# [Postprocessors]
#   # [extdriving]
#   #   type = ElementAverageValue
#   #   variable = 'ce'
#   # []
#   # # [beta_0]
#   # #   type = ADElementAverageMaterialProperty
#   # #   mat_prop = 'beta_0'
#   # # []
#   # [d_avg]
#   #   type = AverageNodalVariableValue
#   #   variable = d
#   # []
# []

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
  #   file_base = surf_Gc2L0.35del1.16_frac
  #   append_date = true
  #   execute_vector_postprocessors_on = final
  # []
  exodus = true
  file_base = surf_Gc2L0.35del1.16_dt1e-2_J5b_noirr_frac
  append_date = true
  print_linear_residuals = false
[]

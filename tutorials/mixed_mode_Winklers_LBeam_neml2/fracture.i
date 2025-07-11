[Mesh]
  [top_half]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 50
    ny = 25
    xmax = 500
    ymin = 0
    ymax = 250
    boundary_id_offset = 0
    boundary_name_prefix = top_half
  []
  [strip]
    type = BoundingBoxNodeSetGenerator
    input = top_half
    new_boundary = disp_strip
    bottom_left = '0 0 0'
    top_right = '30 0 0'
  []
  [top_stitch]
    type = BoundingBoxNodeSetGenerator
    input = strip
    new_boundary = top_stitch
    bottom_left = '250 0 0'
    top_right = '500 0 0'
  []
  [bottom_half]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 25
    ny = 25
    xmin = 250
    xmax = 500
    ymin = -250
    ymax = 0
    boundary_id_offset = 5
    boundary_name_prefix = bottom_half
  []
  [bottom_stitch]
    type = BoundingBoxNodeSetGenerator
    input = bottom_half
    new_boundary = bottom_stitch
    bottom_left = '250 0 0'
    top_right = '500 0 0'
  []
  [stitch]
    type = StitchedMeshGenerator
    inputs = 'top_stitch bottom_stitch'
    stitch_boundaries_pairs = 'top_stitch bottom_stitch'
  []
  [left_bulk]
    type = BoundingBoxNodeSetGenerator
    input = stitch
    new_boundary = left_bulk
    bottom_left = '0 0 0'
    top_right = '200 250 0'
  []
  [right_bulk]
    type = BoundingBoxNodeSetGenerator
    input = left_bulk
    new_boundary = right_bulk
    bottom_left = '250 -250 0'
    top_right = '500 -50 0'
  []
  [up_corner]
    type = BoundingBoxNodeSetGenerator
    input = right_bulk
    new_boundary = up_corner
    bottom_left = '200 0 0'
    top_right = '250 0 0'
  []
  [down_corner]
    type = BoundingBoxNodeSetGenerator
    input = up_corner
    new_boundary = down_corner
    bottom_left = '250 -50 0'
    top_right = '250 0 0'
  []
  construct_side_list_from_node_list = true
[]

[Adaptivity]
  marker = marker
  initial_marker = marker
  initial_steps = 2
  stop_time = 0
  max_h_level = 1
  [Markers]
    [marker]
      type = OrientedBoxMarker
      center = '375 125 0'
      length = 400
      width = 400
      height = 1
      length_direction = '0 250 0'
      width_direction = '250 0 0'
      inside = REFINE
      outside = DO_NOTHING
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
[]

[Bounds]
  [irreversibility]
    type = VariableOldValueBounds
    variable = bounds_dummy
    bounded_variable = d
    bound_type = lower
  []
  [upper]
    type = ConstantBounds
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
    fracture_toughness = ${Gc}
    regularization_length = ${l}
    normalization_constant = 2.6666666667
  []
  [source]
    type = PFFSource
    variable = d
  []
[]

[BCs]
  [left_d_fix]
    type = DirichletBC
    variable = 'd'
    boundary = 'left_bulk'
    value = 0
  []
  [right_d_fix]
    type = DirichletBC
    variable = 'd'
    boundary = 'right_bulk'
    value = 0
  []
  [up_corner_fix]
    type = DirichletBC
    variable = 'd'
    boundary = 'up_corner'
    value = 1
  []
  [down_corner_fix]
    type = DirichletBC
    variable = 'd'
    boundary = 'down_corner'
    value = 1
  []
[]

[NEML2]
  input = 'constitutive.i'
  verbose = true
  device = 'cpu'
  [all]
    model = 'fracture'

    moose_input_types = 'VARIABLE VARIABLE'  
    moose_inputs = 'd psie_active' 
    neml2_inputs = 'forces/d forces/psie0' 

    moose_output_types = 'MATERIAL'
    moose_outputs = 'dpsi_dd'
    neml2_outputs = 'state/dpsi_dd'

    moose_derivative_types = 'MATERIAL'
    moose_derivatives = 'd2psi_dd2'
    neml2_derivatives = 'state/dpsi_dd forces/d'
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
  print_linear_residuals = false
[]

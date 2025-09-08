# Simulated with these commented concrete properties in all related input files (including neml2)
# E = 25.84e3 
# nu = 0.18
# K = '${fparse E/3/(1-2*nu)}'
# G = '${fparse E/2/(1+nu)}'
# psic = 3.44

l = 3.125
Gc = 0.095



[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = fracture.i
    cli_args = 'Gc=${Gc};l=${l}'
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [from_d]
    type = MultiAppCopyTransfer
    from_multi_app = 'fracture'
    variable = d
    source_variable = d
  []
  [to_psie_active]
    type = MultiAppCopyTransfer
    to_multi_app = 'fracture'
    variable = psie_active
    source_variable = psie_active
  []
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Problem]
  extra_tag_vectors = 'ref'
[]

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
  [disp_x]
  []
  [disp_y]
  []
[]

[AuxVariables]
  [fy]
    [AuxKernel]
      type = TagVectorAux
      v = 'disp_y'
      vector_tag = 'ref'
    []
  []
  [d]
  []
  [psie_active]
    order = CONSTANT
    family = MONOMIAL
    [AuxKernel]
      type = MaterialRealAux
      property = psie_active
    []
  []
[]

[Physics]
  [SolidMechanics]
    [QuasiStatic]
      [all]
        strain = SMALL
        formulation = TOTAL
        new_system = true
        add_variables = true
        extra_vector_tags = 'ref'
        generate_output = "cauchy_stress_xx cauchy_stress_xy 
                           cauchy_stress_yx cauchy_stress_yy
													 mechanical_strain_xx mechanical_strain_xy 
													 mechanical_strain_yx mechanical_strain_yy"
      []
    []
  []
[]

[BCs]
  [ydisp]
    type = FunctionDirichletBC
    variable = 'disp_y'
    boundary = 'disp_strip'
    function = 't'
    preset = false
  []
  [yfix]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'bottom_half_bottom'
    value = 0
  []
  [xfix]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'bottom_half_bottom'
    value = 0
  []
[]

[NEML2]
  input = 'constitutive.i'
  verbose = true
  device = 'cpu'
  [all]
    model = 'mechanics'

    moose_input_types = 'MATERIAL VARIABLE'
    moose_inputs = 'neml2_strain d'
    neml2_inputs = 'forces/E forces/d'

    moose_output_types = 'MATERIAL MATERIAL'  
    moose_outputs = 'neml2_stress psie_active' 
    neml2_outputs = 'state/S state/psie_active' 

    moose_derivative_types = 'MATERIAL'
    moose_derivatives = 'neml2_jacobian'
    neml2_derivatives = 'state/S forces/E'
  []
[]

[Materials]
  [convert_strain]
    type = RankTwoTensorToSymmetricRankTwoTensor
    from = 'mechanical_strain'
    to = 'neml2_strain'
  []
  [stress]
    type = ComputeLagrangianObjectiveCustomSymmetricStress
    custom_small_stress = 'neml2_stress'
    custom_small_jacobian = 'neml2_jacobian'
  []
[]

[Postprocessors]
  [Fy]
    type = NodalSum
    variable = fy
    boundary = disp_strip
    execute_on = 'TIMESTEP_END'
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

  dt = 0.0001
  end_time = 2

  fixed_point_max_its = 500
  accept_on_max_fixed_point_iteration = false
  fixed_point_rel_tol = 1e-8
  fixed_point_abs_tol = 1e-8
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

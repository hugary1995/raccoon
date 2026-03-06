Gc = 2.7
l = 0.02
c0 = 2
scale = '${fparse 1/c0/l}'

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 30
    ny = 15
    ymax = 0.5
  []
  [noncrack]
    type = BoundingBoxNodeSetGenerator
    input = gen
    new_boundary = noncrack
    bottom_left = '0.5 0 0'
    top_right = '1 0 0'
  []
  construct_side_list_from_node_list = true
[]

[Adaptivity]
  marker = marker
  initial_marker = marker
  initial_steps = 2
  stop_time = 0
  max_h_level = 2
  [Markers]
    [marker]
      type = BoxMarker
      bottom_left = '0.4 0 0'
      top_right = '1 0.05 0'
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
    normalization_constant = ${c0}
  []
  [source]
    type = PFFSource
    variable = d
    free_energy = 'psi'
  []
[]

[NEML2]
  input = 'fracture_constitutive.i'
  cli_args = 'scale=${scale}'
  verbose = true
  device = 'cpu'
  [all]
    model = 'model'

    moose_input_types = 'VARIABLE VARIABLE'
    moose_inputs = 'd psie_active'
    neml2_inputs = 'forces/d forces/psie_active'

    moose_parameter_types = 'POSTPROCESSOR'
    moose_parameters = '${Gc}'
    neml2_parameters = 'crack_surface_energy_c_0'

    moose_output_types = 'MATERIAL'
    moose_outputs = 'dpsi/dd'
    neml2_outputs = 'state/m'

    moose_derivative_types = 'MATERIAL'
    moose_derivatives = 'd^2psi/dd^2'
    neml2_derivatives = 'state/m forces/d'
  []
[]

[Debug]
  show_material_props = true
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

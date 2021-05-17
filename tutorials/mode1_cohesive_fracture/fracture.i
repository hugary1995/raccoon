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
    prop_names = 'Gc psic l'
    prop_values = '${Gc} ${psic} ${l}'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    phase_field = d
  []
  [degradation]
    type = RationalDegradationFunction
    f_name = g
    function = (1-d)^p/((1-d)^p+(Gc/psic*xi/c0/l)*d*(1+a2*d+a2*a3*d^2))*(1-eta)+eta
    phase_field = d
    material_property_names = 'Gc psic xi c0 l '
    parameter_names = 'p a2 a3 eta '
    parameter_values = '2 -0.5 0 1e-6'
  []
  [psi]
    type = ADDerivativeParsedMaterial
    f_name = psi
    function = 'alpha*Gc/c0/l+g*psie_active'
    args = 'd psie_active'
    material_property_names = 'alpha(d) g(d) Gc c0 l'
    derivative_order = 1
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

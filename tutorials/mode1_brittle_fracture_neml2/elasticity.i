Gc = 2.7
l = 0.02

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
        new_system = true
        add_variables = true
        extra_vector_tags = 'ref'
      []
    []
  []
[]

[BCs]
  [ydisp]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = top
    function = 't'
  []
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = noncrack
    value = 0
  []
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = top
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
    boundary = top
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

  dt = 1e-4
  end_time = 0.0036

  reuse_preconditioner = true

  fixed_point_max_its = 20
  accept_on_max_fixed_point_iteration = true
  fixed_point_rel_tol = 1e-8
  fixed_point_abs_tol = 1e-10
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

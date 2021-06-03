E = 2.1e5
nu = 0.3
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

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
    multi_app = fracture
    direction = from_multiapp
    variable = d
    source_variable = d
  []
  [to_psie]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    variable = 'psie invar_1 invar_2'
    source_variable ='psie invar_1 invar_2'
  []
[]

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 10
    ny = 10
    nz = 10
    # ymax = 0.5
  []
  # [noncrack]
  #   type = BoundingBoxNodeSetGenerator
  #   input = gen
  #   new_boundary = noncrack
  #   bottom_left = '0.5 0 0'
  #   top_right = '1 0 0'
  # []
  # construct_side_list_from_node_list = true
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
  [disp_x]
  []
  [disp_y]
  []
  [disp_z]
  []
[]

[AuxVariables]
  [fy]
  []
  [d]
  []
  [invar_1]
    # order = CONSTANT
    # family = MONOMIAL
  []
  [invar_2]
    # order = CONSTANT
    # family = MONOMIAL
  []
  [sigma_00]
    # order = CONSTANT
    # family = MONOMIAL
  []
  [sigma_11]
    # order = CONSTANT
    # family = MONOMIAL
  []
  [sigma_22]
    # order = CONSTANT
    # family = MONOMIAL
  []
[]

[AuxKernels]
  [invar_1]
    type = ADRankTwoScalarAux
    rank_two_tensor = stress
    variable = invar_1
    scalar_type = firstinvariant
  []
  [invar_2]
    type = ADRankTwoScalarAux
    rank_two_tensor = stress
    variable = invar_2
    scalar_type = secondinvariant
  []
  [sigma_000]
    type = ADRankTwoAux
    index_i = 0
    index_j = 0
    rank_two_tensor = 'stress'
    # property = 'stress'
    variable = 'sigma_00'
  []
  [sigma_110]
    type = ADRankTwoAux
    index_i = 1
    index_j = 1
    rank_two_tensor = 'stress'
    variable = 'sigma_11'
  []
  [sigma_220]
    type = ADRankTwoAux
    index_i = 2
    index_j = 2
    rank_two_tensor = 'stress'
    variable = 'sigma_22'
  []
[]

[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    displacements = 'disp_x disp_y disp_z'
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    displacements = 'disp_x disp_y disp_z'
  []
  [solid_z]
    type = ADStressDivergenceTensors
    variable = disp_z
    component = 2
    displacements = 'disp_x disp_y disp_z'
  []
[]

[BCs]
  [./symmy]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
  [../]
  # [./udisp]
  #   type = DirichletBC
  #   variable = disp_y
  #   boundary = top
  #   value = 2e-4
  # [../]
  [./symmx]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  [../]
  [./rdisp]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = right
    function = 't'
  [../]
  [./symmz]
    type = DirichletBC
    variable = disp_z
    boundary = back
    value = 0
  [../]
  # [./tdisp]
  #   type = DirichletBC
  #   variable = disp_z
  #   boundary = front
  #   value = 3e-4
  # [../]
[]

[Materials]
  [bulk]
    type = ADGenericConstantMaterial
    prop_names = 'K G'
    prop_values = '${K} ${G}'
  []
  [degradation]
    type = PowerDegradationFunction
    f_name = g
    function = (1-d)^p*(1-eta)+eta
    phase_field = d
    parameter_names = 'p eta '
    parameter_values = '2 0'
  []
  [strain]
    type = ADComputeSmallStrain
  []
  [elasticity]
    type = SmallDeformationIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    decomposition = NONE
    output_properties = 'elastic_strain psie'
    outputs = exodus
  []
  [stress]
    type = ComputeSmallDeformationStress
    elasticity_model = elasticity
    output_properties = 'stress'
    outputs = exodus
  []
  # [sigma_00]
  #   type = ADRankTwoCartesianComponent
  #   index_i = 0
  #   index_j = 0
  #   property_name = 'sigma_00'
  #   rank_two_tensor = 'stress'
  # []
[]

[Postprocessors]
  [Fy]
    type = NodalSum
    variable = fy
    boundary = top
  []
  [psie]
    type = ADElementAverageMaterialProperty
    mat_prop = psie
  []
  # [sigma_00_0]
  #   type = ElementalVariableValue
  #   elementid = 0
  #   variable = sigma_00
  # []
[]

[VectorPostprocessors]
  [nodal]
    type = NodalValueSampler
    variable = 'disp_x disp_y disp_z sigma_00 sigma_11 sigma_22 invar_1 invar_2'
    sort_by = id
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

  dt = 2e-4
  end_time = 3.5e-3

  picard_max_its = 20
  accept_on_max_picard_iteration = true
  picard_rel_tol = 1e-8
  picard_abs_tol = 1e-10
[]

[Outputs]
  [csv_]
type = CSV
file_base = linear_ela
append_date = true
#show = 'var_u'
execute_vector_postprocessors_on = final
[]
  exodus = true
  file_base = linear_ela
  print_linear_residuals = false
[]

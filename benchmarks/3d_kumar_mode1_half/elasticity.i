E = 9.8e3 #9.8Gpa
nu = 0.13
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'
Lambda = '${fparse E*nu/(1+nu)/(1-2*nu)}'

Gc = 9.1e-4 # 91N/m
l = 0.120
sigma_ts = 27
sigma_cs = 77
delta = 9.66

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = fracture.i
    cli_args = 'Gc=${Gc};l=${l};G=${G};Lambda=${Lambda};sigma_ts=${sigma_ts};sigma_cs=${sigma_cs};delta=${delta}'
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [from_d]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    variable = 'd' # F_surface beta_0 beta_1 beta_2 beta_3 J2'
    source_variable = 'd' # F_surface beta_0 beta_1 beta_2 beta_3 J2'
  []
  [to_psie_active]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    variable = 'psie_active ce' # invar_1 invar_2'
    source_variable ='psie_active ce' #invar_1 invar_2'
  []
[]

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Mesh]
  [top_half]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 50
    ny = 25
    nz = 1
    xmax = 1
    ymin = 0
    ymax = 0.5
    zmax = 0.020
    boundary_id_offset = 0
    boundary_name_prefix = top_half
  []
  [noncrack]
    type = BoundingBoxNodeSetGenerator
    input = top_half
    new_boundary = noncrack
    bottom_left = '0.5 0 0'
    top_right = '1 0 0.02'
  []
  construct_side_list_from_node_list = true
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
#       top_right = '1 0.2 0.5'
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
  # [ce0]
  #   # order = CONSTANT
  #   # family = MONOMIAL
  # []
  # [F_surface]
  #   family = MONOMIAL
  # []
  # [beta_0]
  #   family = MONOMIAL
  # []
  # [beta_1]
  #   family = MONOMIAL
  # []
  # [beta_2]
  #   family = MONOMIAL
  # []
  # [beta_3]
  #   family = MONOMIAL
  # []
  # [J2]
  #   family = MONOMIAL
  # []
[]


[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    # displacements = 'disp_x disp_y disp_z'
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    # displacements = 'disp_x disp_y disp_z'
    save_in = fy
  []
  [solid_z]
    type = ADStressDivergenceTensors
    variable = disp_z
    component = 2
    # displacements = 'disp_x disp_y disp_z'
  []
[]

[BCs]
  # [left_x]
  #   type = DirichletBC
  #   variable = disp_x
  #   boundary = left
  #   value = 0
  # []
  # [left_y]
  #   type = DirichletBC
  #   variable = disp_y
  #   boundary = left
  #   value = 0
  # []
  # [left_z]
  #   type = DirichletBC
  #   variable = disp_z
  #   boundary = top_half_left
  #   value = 0
  # []
  # [right_x]
  #   type = DirichletBC
  #     variable = disp_x
  #     boundary = 'top_half_right bottom_half_right'
  #     value = 0
  # []
  # [right_y]
  #   type = DirichletBC
  #   variable = disp_y
  #   boundary = 'top_half_right bottom_half_right'
  #   value = 0
  # []
  # [right_z]
  #   type = DirichletBC
  #   variable = disp_z
  #   boundary = top_half_right
  #   value = 0
  # []
  [bottom_x]
    type = DirichletBC
    variable = disp_x
    boundary = noncrack
    value = 0
  []
  [bottom_y]
    type = DirichletBC
    variable = disp_y
    boundary = noncrack
    value = 0
  []
  [bottom_z]
    type = DirichletBC
    variable = disp_z
    boundary = noncrack
    value = 0
  []
  # [top_x]
  #   type = DirichletBC
  #   variable = disp_x
  #   boundary = top
  #   value = 0
  # []
  [top_y]
    type = FunctionDirichletBC
      variable = disp_y
      boundary = top_half_top
      function = 't'
  []
  # [top_z]
  #   type = DirichletBC
  #   variable = disp_z
  #   boundary = top_half_top
  #   value = 0
  # []
  # [back_x]
  #   type = DirichletBC
  #   variable = disp_x
  #   boundary = noncrack
  #   value = 0
  # []
  # [back_y]
  #   type = DirichletBC
  #   variable = disp_y
  #   boundary = noncrack
  #   value = 0
  # []
  # [back_z]
  #   type = DirichletBC
  #   variable = disp_z
  #   boundary = top_half_back
  #   value = 0
  # []
  # [front_x]
  #   type = DirichletBC
  #   variable = disp_x
  #   boundary = front
  #   value = 0
  # []
  # [front_y]
  #   type = DirichletBC
  #   variable = disp_y
  #   boundary = front
  #   value = 0
  # []
  # [front_z]
  #   type = DirichletBC
  #   variable = disp_z
  #   boundary = top_half_front
  #   value = 0
  # []
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
    output_properties = 'elastic_strain psie_active'
    outputs = exodus
  []
  [stress]
    type = ComputeSmallDeformationStress
    elasticity_model = elasticity
    output_properties = 'stress'
    outputs = exodus
  []
  [I1]
    type = ADRankTwoInvariant
    property_name = invar_1
    rank_two_tensor = 'stress'
    invariant = FirstInvariant
    output_properties = 'invar_1'
    outputs = exodus
  []
  [I2]
    type = ADRankTwoInvariant
    property_name = invar_2
    rank_two_tensor = 'stress'
    invariant = SecondInvariant
    output_properties = 'invar_2'
    outputs = exodus
  []
  [kumar_material]
    type = GeneralizedExternalDrivingForce
    # first_invariant = invar_1
    # second_invariant = invar_2
    tensile_strength = '${sigma_ts}' #27MPa
    compressive_strength = '${sigma_cs}' #77MPa
    delta = '${delta}'
    energy_release_rate = '${Gc}'
    phase_field_regularization_length = '${l}'
    Lame_first_parameter = '${Lambda}'
    shear_modulus = '${G}'
    external_driving_force_name = ce
    output_properties = 'F_surface J2 beta_0 beta_1 beta_2 beta_3 ce'
    outputs = exodus
  []
[]

[Postprocessors]
  [psie_active]
    type = ADElementAverageMaterialProperty
    mat_prop = psie_active
  []
  [ce]
    type = ADElementAverageMaterialProperty
    mat_prop = ce
  []
  [Fy]
    type = NodalSum
    variable = fy
    boundary = top_half_top
  []
[]

[VectorPostprocessors]
  [nodal]
    type = NodalValueSampler
    variable = 'd disp_x disp_y disp_z' # sigma_00 sigma_11 sigma_22 invar_1 invar_2'
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

  dt = 1e-4
  end_time = 1e-3

  picard_max_its = 20
  accept_on_max_picard_iteration = true
  picard_rel_tol = 1e-8
  picard_abs_tol = 1e-10
[]

[Outputs]
  [csv_]
type = CSV
file_base = kumar_mode1_half_Gc4L0.12del9.66z1ele_ela
append_date = true
execute_vector_postprocessors_on = final
[]
  exodus = true
  file_base = kumar_mode1_half_Gc4L0.12del9.66z1ele
  append_date = true
  print_linear_residuals = false
[]

E = 9.8e3 #9.8Gpa
nu = 0.13
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'
Lambda = '${fparse E*nu/(1+nu)/(1-2*nu)}'

Gc = 9.1e-2 # 91N/m
l = 0.35
sigma_ts = 27
sigma_cs = 77
delta = 1.16
c1 = '${fparse (1+nu)*sqrt(Gc)/sqrt(2*pi*E)}'
c2 = '${fparse (3-nu)/(1-nu)}'


[Functions]
  [bc_func]
    type = ParsedFunction
    value = c1*((x-20*t)^2+y^2)^(1/4)*(c2-cos(atan(y/(x-20*t))))*sin(1/2*atan(y/(x-20*t)))
    vars = 'c1 c2'
    vals = '${c1} ${c2}'
  []
[]

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
  displacements = 'disp_x disp_y'
[]

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
  [disp_x]
  []
  [disp_y]
  []
  # [disp_z]
  # []
[]

[AuxVariables]
  [fy]
  []
  [d]
  []
  # [bcfunc]
  # []
[]


[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    save_in = fy
  []
  # [solid_z]
  #   type = ADStressDivergenceTensors
  #   variable = disp_z
  #   component = 2
  #   # displacements = 'disp_x disp_y disp_z'
  # []
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
  # [bottom_x]
  #   type = DirichletBC
  #   variable = disp_x
  #   boundary = bottom_half_bottom
  #   value = 0
  # []
  # [bottom_y]
  #   type = DirichletBC
  #   variable = disp_y
  #   boundary = bottom_half_bottom
  #   value = 0
  # []
  [bottom_y]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = bottom_half_bottom
    function = bc_func
  []
  # [bottom_z]
  #   type = DirichletBC
  #   variable = disp_z
  #   boundary = bottom_half_bottom
  #   value = 0
  # []
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
    function = bc_func
    # save_in = bcfunc
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
    output_properties = 'psie_active' #elastic_strain
    outputs = exodus
  []
  [stress]
    type = ComputeSmallDeformationStress
    elasticity_model = elasticity
    output_properties = 'stress'
    outputs = exodus
  []
  [kumar_material]
    type = GeneralizedExternalDrivingForce
    rank_two_tensor = 'stress'
    tensile_strength = '${sigma_ts}' #27MPa
    compressive_strength = '${sigma_cs}' #77MPa
    delta = '${delta}'
    energy_release_rate = '${Gc}'
    phase_field_regularization_length = '${l}'
    Lame_first_parameter = '${Lambda}'
    shear_modulus = '${G}'
    external_driving_force_name = ce
    output_properties = 'ce' #'F_surface J2 beta_0 beta_1 beta_2 beta_3 ce'
    outputs = exodus
  []
  [pk1]
    type =RankTwoTensorMaterialConverter
    ad_props_in = stress
    ad_props_out = pk1_stress
    intra_convert = true
  []
  #   type = ADGenericFunctionRankTwoTensor
  #   tensor_name = pk1_stress
  #   tensor_functions = 'stress_00 stress_01 stress_02 stress_10 stress_11 stress_12 stress_20 stress_21 stress_22'
  #   # outputs = all
  # []
[]

[Postprocessors]
  # [psie_active]
  #   type = ADElementAverageMaterialProperty
  #   mat_prop = psie_active
  # []
  # [ce]
  #   type = ADElementAverageMaterialProperty
  #   mat_prop = ce
  # []
  [Fy]
    type = NodalSum
    variable = fy
    boundary = top_half_top
  []
  [Jint]
    type = PhaseFieldJIntegral
    J_direction = '1 0 0'
    elastic_energy_name = psie_active
    displacements = 'disp_x disp_y'
    boundary = 'top_half_left top_half_top top_half_right bottom_half_right bottom_half_bottom bottom_half_left'
  []
[]

# [VectorPostprocessors]
#   [nodal]
#     type = NodalValueSampler
#     variable = 'd disp_x disp_y disp_z' # sigma_00 sigma_11 sigma_22 invar_1 invar_2'
#     sort_by = id
#   []
# []

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist                 '
  automatic_scaling = true

  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10

  dt = 1e-2
  end_time = 1

  picard_max_its = 20
  accept_on_max_picard_iteration = true
  picard_rel_tol = 1e-8
  picard_abs_tol = 1e-10
[]

[Outputs]
#   [csv_]
# type = CSV
# file_base = surf_Gc2L0.35del1.16_ela
# append_date = true
# execute_vector_postprocessors_on = final
# []
  exodus = true
  file_base = surf_Gc2L0.35del1.16_dt1e-2_J5b_noirr_ela
  append_date = true
  print_linear_residuals = false
[]

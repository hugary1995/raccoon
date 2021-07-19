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
    variable = 'd'
    source_variable = 'd'
  []
  [to_psie_active]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    variable = 'psie_active'
    source_variable ='psie_active'
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
    xmax = 30
    ymin = 0
    ymax = 5
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
    top_right = '5 0.36 0'
  []
  [top_stitch]
    type = BoundingBoxNodeSetGenerator
    input = createNewSidesetOne
    new_boundary = top_stitch
    bottom_left = '5 0 0'
    top_right = '30 0 0'
  []
  [bottom_half]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 84 #750/8
    ny = 14 #125/8
    xmax = 30
    ymin = -5
    ymax = 0
    boundary_id_offset = 7
    boundary_name_prefix = bottom_half
  []
  [createNewSidesetTwo]
    type = SideSetsFromBoundingBoxGenerator
    input = bottom_half
    block_id = 0
    boundary_id_old = 'bottom_half_top'
    boundary_id_new = 200
    bottom_left = '0 0 0'
    top_right = '5 -0.36 0'
  []
  [bottom_stitch]
    type = BoundingBoxNodeSetGenerator
    input = createNewSidesetTwo
    new_boundary = bottom_stitch
    bottom_left = '5 0 0'
    top_right = '30 0 0'
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
  initial_steps = 3
  stop_time = 0
  max_h_level = 3
  [Markers]
    [marker]
      type = BoxMarker
      bottom_left = '4.2 -0.8 0'
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
[]

[AuxVariables]
  [fy]
  []
  [d]
  []
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
[]

[BCs]
  [bottom_y]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = bottom_half_bottom
    function = bc_func
  []
  [top_y]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = top_half_top
    function = bc_func
  []
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
  [pk1]
    type =RankTwoTensorMaterialConverter
    ad_props_in = stress
    ad_props_out = pk1_stress
    intra_convert = true
  []
[]

[Postprocessors]
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
  exodus = true
  file_base = surf_AT2_Gc2L0.35del1.16_dt1e-2_J5b_noirr_ela
  append_date = true
  print_linear_residuals = false
[]

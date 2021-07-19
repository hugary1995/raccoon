E = 9.8e3 #9.8Gpa
nu = 0.13
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'
Lambda = '${fparse E*nu/(1+nu)/(1-2*nu)}'

Gc = 9.1e-2 # 91N/m
l = 0.350
sigma_ts = 27
sigma_cs = 77
delta = 1.16

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
    variable = d
    source_variable = d
  []
  [to_psie_active]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    variable = 'psie_active ce'
    source_variable ='psie_active ce'
  []
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = 'bar.msh'
  []
  # [gen]
  # type = GeneratedMeshGenerator
  # dim = 1
  # nx = 44
  # xmax = 1
  # xmin = -1
  # []
  # [mid]
  #   type = BoundingBoxNodeSetGenerator
  #   input = gen
  #   new_boundary = mid
  #   bottom_left = '0 0 0'
  #   top_right = '0 0 0' #30
  #   # show_info = true
  # []
  # construct_side_list_from_node_list = true
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
[]

[AuxVariables]
  [d]
    # [InitialCondition]
    #   type = FunctionIC
    #   function = 'if(x=0,0.5,0)'
    # []
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
  []
[]

[BCs]
  [left_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'left'
    value = 0
  []
  [right_x]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 'right'
    function = 't'
  []
  # [mid_d]
  #   type = DirichletBC
  #   variable = d
  #   boundary = mid
  #   value = 0.5
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
[]

# [Postprocessors]
#   [psie_active]
#     type = ADElementAverageMaterialProperty
#     mat_prop = psie_active
#   []
# []
#
# [VectorPostprocessors]
#   [nodal]
#     type = NodalValueSampler
#     variable = 'd disp_x' # sigma_00 sigma_11 sigma_22 invar_1 invar_2'
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

  dt = 1e-6
  start_time = 0.02151
  end_time = 0.02161

  picard_max_its = 20
  accept_on_max_picard_iteration = true
  picard_rel_tol = 1e-8
  picard_abs_tol = 1e-10
[]

[Outputs]
#   [csv_]
# type = CSV
# file_base = 1d_kumar_ela_m20L0.05
# append_date = true
# #show = 'var_u'
# execute_vector_postprocessors_on = final
# []
  exodus = true
  file_base = bar_L0.35del1.16_noirr_nuc_ela
  append_date = true
  print_linear_residuals = false
[]

E = 1
nu = 0.3
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

gapK = 5e-3

Gc = 1
l = 0.025

refine = 3
h = '${fparse 0.1/2^refine}'
l_over_h = '${fparse int(l/h)}'
dls = '${fparse (1-(2*l-h)/(2*l))^2}'

a = 0.8

[MultiApps]
  [fracture]
    type = FullSolveMultiApp
    input_files = fracture.i
    cli_args = 'Gc=${Gc};l=${l};refine=${refine};a=${a}'
    execute_on = 'INITIAL'
  []
  [levelset]
    type = TransientMultiApp
    input_files = levelset.i
    cli_args = 'refine=${refine};h=${h};l_over_h=${l_over_h};dls=${dls};l=${l}'
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [fracture]
    type = MultiAppCopyTransfer
    from_multi_app = fracture
    variable = d
    source_variable = d
  []
  [to_levelset]
    type = MultiAppCopyTransfer
    to_multi_app = levelset
    variable = 'd disp_x disp_y T'
    source_variable = 'd disp_x disp_y T'
  []
  [from_levelset]
    type = MultiAppCopyTransfer
    from_multi_app = levelset
    variable = 'w wT_x wT_y'
    source_variable = 'w wT_x wT_y'
  []
  [from_levelset_phi]
    type = MultiAppMeshFunctionTransfer
    from_multi_app = levelset
    variable = 'phi'
    source_variable = 'phi'
  []
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 10
    ny = 10
  []
  [mid]
    type = SubdomainBoundingBoxGenerator
    input = gen
    block_id = 1
    bottom_left = '0 0.45 0'
    top_right = '1 0.55 0'
  []
  [refine]
    type = RefineBlockGenerator
    input = mid
    block = 1
    refinement = ${refine}
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [T]
  []
[]

[AuxVariables]
  [d]
  []
  [phi]
  []
  [w]
  []
  [wT_x]
  []
  [wT_y]
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
  [cond]
    type = ADHeatConduction
    variable = T
    thermal_conductivity = kappa
  []
  [gap_transfer]
    type = ADPFFGapHeatTransfer
    variable = T
    T_jump = 'wT_x wT_y'
    u_jump = w
    gap_conductivity = gapK
    phase_field = d
    level_set = phi
    min_gap = 1e-3
  []
[]

[BCs]
  [ydisp]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'top'
    function = 't'
  []
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  []
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = 'top bottom'
    value = 0
  []
  [temp_bottom]
    type = DirichletBC
    variable = T
    boundary = 'bottom'
    value = 0
  []
  [temp_top]
    type = DirichletBC
    variable = T
    boundary = 'top'
    value = 100
  []
[]

[Materials]
  [bulk_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G gapK'
    prop_values = '${K} ${G} ${gapK}'
  []
  [degradation]
    type = PowerDegradationFunction
    f_name = g
    function = (1-d)^p*(1-eta)+eta
    phase_field = d
    parameter_names = 'p eta '
    parameter_values = '2 1e-8'
  []
  [thermal_conductivity]
    type = ADParsedMaterial
    f_name = kappa
    function = 'g*kappa0'
    constant_names = 'kappa0'
    constant_expressions = '1'
    material_property_names = 'g'
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
  []
  [stress]
    type = ComputeSmallDeformationStress
    elasticity_model = elasticity
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

  dt = 0.01
  num_steps = 1

  fixed_point_algorithm = picard
  fixed_point_max_its = 20
  fixed_point_rel_tol = 1e-08
  fixed_point_abs_tol = 1e-10
  accept_on_max_fixed_point_iteration = true
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

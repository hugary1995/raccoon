E = 2.1e5
nu = 0.3
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

Gc = 27
sigma_cs = 20000
sigma_ts = 2500
gamma_1 = '${fparse (sigma_cs-sigma_ts)/sqrt(3)/(sigma_cs+sigma_ts)}'
gamma_0 = '${fparse 2*sigma_cs*sigma_ts/sqrt(3)/(sigma_cs+sigma_ts)}'
l = 0.1
psic = 100
eta = 1e-8

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = fracture.i
    cli_args = 'a=${a};Gc=${Gc};psic=${psic};l=${l};eta=${eta}'
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [from_d]
    type = MultiAppMeshFunctionTransfer
    multi_app = fracture
    direction = from_multiapp
    variable = d
    source_variable = d
  []
  [to_psi_active]
    type = MultiAppMeshFunctionTransfer
    multi_app = fracture
    direction = to_multiapp
    variable = 'psin_active psie_active'
    source_variable = 'psin_active psie_active'
  []
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

# [Mesh]
#   [fmg]
#     type = FileMeshGenerator
#     file = gold/${a}.msh
#   []
# []

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 20
    ny = 30
    xmax = 5
    ymax = 7.5
  []
  [noncrack]
    type = BoundingBoxNodeSetGenerator
    input = gen
    new_boundary = noncrack
    bottom_left = '${a} 0 0'
    top_right = '5 0 0'
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
      bottom_left = '${fparse a-0.25} 0 0'
      top_right = '${fparse a+1} 0.2 0'
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
  [psin_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [psie_active]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [psin_active]
    type = ADMaterialRealAux
    variable = psin_active
    property = psin_active
  []
  [psie_active]
    type = ADMaterialRealAux
    variable = psie_active
    property = psie_active
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
    boundary = left
    value = 0
  []
[]

[Materials]
  [bulk_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G l Gc psic'
    prop_values = '${K} ${G} ${l} ${Gc} ${psic}'
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
    parameter_values = '2 1 0 ${eta}'
  []
  # [degradation]
  #   type = PowerDegradationFunction
  #   f_name = g
  #   function = (1-d)^p*(1-eta)+eta
  #   phase_field = d
  #   parameter_names = 'p eta '
  #   parameter_values = '2 ${eta}'
  # []
  [strain]
    type = ADComputeSmallStrain
  []
  [stress]
    type = ComputeStressWithStrengthSurface
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    gamma_1 = ${gamma_1}
    gamma_0 = ${gamma_0}
    structure_energy_density = psin
    strain_energy_density = psie
  []
  [gamma]
    type = CrackSurfaceDensity
    crack_geometric_function = alpha
    crack_surface_density = gamma
    normalization_constant = c0
    phase_field = d
    regularization_length = l
  []
  [psi_active]
    type = ADParsedMaterial
    f_name = psi_active
    function = 'psie_active+psin_active'
    material_property_names = 'psie_active psin_active'
  []
[]

[Postprocessors]
  [Fy]
    type = NodalSum
    variable = fy
    boundary = top
  []
  [Gamma]
    type = ADElementIntegralMaterialProperty
    mat_prop = gamma
  []
  [psie]
    type = ADElementIntegralMaterialProperty
    mat_prop = psie
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
  end_time = 1

  picard_max_its = 20
  accept_on_max_picard_iteration = true
  picard_rel_tol = 1e-8
  picard_abs_tol = 1e-10
[]

[Outputs]
  file_base = 'a_${a}'
  csv = true
  exodus = true
  print_linear_residuals = false
[]

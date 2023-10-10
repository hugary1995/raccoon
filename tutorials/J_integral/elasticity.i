E = 3e4
nu = 0.2
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

Gc = 0.12
l = 0.125
h = '${fparse sqrt(2)*l/4}'

W = '${fparse 80*l}'
H = '${fparse 20*l}'
nx = '${fparse ceil(W/h/8)}'
ny = '${fparse ceil(H/h/8)}'
refine = 3

p = 3

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = fracture.i
    cli_args = 'K=${K};G=${G};Gc=${Gc};l=${l};W=${W};H=${H};nx=${nx};ny=${ny};refine=${refine}'
    execute_on = 'INITIAL TIMESTEP_END'
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
    source_variable = 'psie_active'
  []
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    xmax = ${W}
    ymax = ${H}
    nx = ${nx}
    ny = ${ny}
  []
[]

[Adaptivity]
  marker = marker
  initial_marker = marker
  initial_steps = ${refine}
  stop_time = 0
  max_h_level = ${refine}
  [Markers]
    [marker]
      type = BoxMarker
      bottom_left = '0 0 0'
      top_right = '${fparse W/2} ${fparse 4*l} 0'
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
  [d]
  []
  [psie_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [q]
    [InitialCondition]
      type = BoundingBoxIC
      x1 = '${fparse 0.1*W}'
      y1 = 0
      x2 = '${fparse 0.4*W}'
      y2 = '${fparse 3*l}'
      inside = 1
      outside = 0
    []
  []
[]

[AuxKernels]
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
  []
  [pressure_x]
    type = ADPressurizedCrack
    variable = disp_x
    component = 0
    indicator_function = I
    phase_field = d
    pressure = p
  []
  [pressure_y]
    type = ADPressurizedCrack
    variable = disp_y
    component = 1
    indicator_function = I
    phase_field = d
    pressure = p
  []
[]

[BCs]
  [symmetry]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  []
  [surfing_x]
    type = ModeISurfingDirichletBC
    variable = disp_x
    boundary = 'left right top'
    G = ${G}
    K = ${K}
    Gc = ${Gc}
    component = 0
    initial_crack_tip_position = '${fparse 0.2*W} 0 0'
    crack_propagation_velocity = '${fparse 0.05*W} 0 0'
  []
  [surfing_y]
    type = ModeISurfingDirichletBC
    variable = disp_y
    boundary = 'left right top'
    G = ${G}
    K = ${K}
    Gc = ${Gc}
    component = 1
    initial_crack_tip_position = '${fparse 0.2*W} 0 0'
    crack_propagation_velocity = '${fparse 0.05*W} 0 0'
  []
[]

[Materials]
  [bulk]
    type = ADGenericConstantMaterial
    prop_names = 'K G Gc l p'
    prop_values = '${K} ${G} ${Gc} ${l} ${p}'
  []
  [indicator]
    type = ADDerivativeParsedMaterial
    f_name = I
    function = 'd'
    args = d
    derivative_order = 1
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd^2'
    phase_field = d
  []
  [degradation]
    type = PowerDegradationFunction
    phase_field = d
    f_name = g
    function = (1-d)^p*(1-eta)+eta
    parameter_names = 'p eta '
    parameter_values = '2 1e-9'
  []
  [strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
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
    output_properties = 'stress'
  []
  [crack_surface_density]
    type = CrackSurfaceDensity
    crack_geometric_function = alpha
    crack_surface_density = gamma
    normalization_constant = c0
    phase_field = d
    regularization_length = l
  []
[]

[Postprocessors]
  [Jint]
    type = JIntegralDomainForm
    domain = q
    strain_energy_density = psie
    J_direction = '1 0 0'
  []
  [Jint_p]
    type = JIntegralDomainFormPressureWork
    domain = q
    phase_field = d
    indicator_function = I
    pressure = p
    J_direction = '1 0 0'
  []
  [JR]
    type = ParsedPostprocessor
    function = '(Jint+Jint_p)/${fparse Gc/2}'
    pp_names = 'Jint Jint_p'
  []
  [crack_length_half]
    type = ADElementIntegralMaterialProperty
    mat_prop = gamma
    outputs = none
  []
  [crack_length]
    type = ParsedPostprocessor
    function = '2*crack_length_half'
    pp_names = 'crack_length_half'
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

  dt = 0.025
  end_time = 3

  fixed_point_max_its = 20
  accept_on_max_fixed_point_iteration = true
  fixed_point_rel_tol = 1e-8
  fixed_point_abs_tol = 1e-10
[]

[Outputs]
  file_base = 'p_${p}_l_${l}'
  exodus = true
  print_linear_residuals = false
[]

a = 1
ratio = 4

beta = 0.5
ep0 = 0.5

psic = 0.9
Gc = 23.6
l = '${fparse ratio * a}'

E = 201.8e3
nu = 0.3
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

rate = 1000
T_init = 300

rho = 7.9e-9
cv = 4.47e7
k = 44

tqf = 0.9

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  volumetric_locking_correction = true
[]

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    cli_args = 'a=${a};psic=${psic};Gc=${Gc};l=${l};'
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [from_fracture]
    type = MultiAppCopyTransfer
    from_multi_app = fracture
    variable = d
    source_variable = d
  []
  [to_fracture]
    type = MultiAppCopyTransfer
    to_multi_app = fracture
    variable = 'psie_active psip_active'
    source_variable = 'psie_active psip_active'
  []
  [to_sub_coalescence]
    type = MultiAppCopyTransfer
    to_multi_app = fracture
    source_variable = coalescence
    variable = coalescence
  []
[]

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 3
    xmax = ${a}
    ymax = ${a}
    zmax = ${a}
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [disp_z]
  []
  [T]
    initial_condition = ${T_init}
  []
[]

[AuxVariables]
  [d]
  []
  [stress]
    order = CONSTANT
    family = MONOMIAL
  []
  [F]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [stress]
    type = ADRankTwoAux
    variable = stress
    rank_two_tensor = stress
    index_i = 1
    index_j = 1
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [F]
    type = ADRankTwoAux
    variable = F
    rank_two_tensor = deformation_gradient
    index_i = 1
    index_j = 1
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]

[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    use_displaced_mesh = true
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    use_displaced_mesh = true
  []
  [solid_z]
    type = ADStressDivergenceTensors
    variable = disp_z
    component = 2
    use_displaced_mesh = true
  []
  [hcond_time]
    type = HeatConductionTimeDerivative
    variable = T
    density_name = density
    specific_heat = specific_heat
  []
  [hcond]
    type = HeatConduction
    variable = T
    diffusion_coefficient = thermal_conductivity
  []
  [heat_source]
    type = ADCoefMatSource
    variable = T
    coefficient = -1
    prop_names = 'plastic_heat_generation'
  []
[]

[Materials]
  [thermal_properties]
    type = GenericConstantMaterial
    prop_names = 'density specific_heat thermal_conductivity'
    prop_values = '${rho} ${cv} ${k}'
  []
  [bulk_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G l Gc psic'
    prop_values = '${K} ${G} ${l} ${Gc} ${psic}'
  []
  [degradation]
    type = RationalDegradationFunction
    property_name = g
    expression = (1-d)^p/((1-d)^p+(Gc/psic*xi/c0/l)*d*(1+a2*d+a2*a3*d^2))*(1-eta)+eta
    phase_field = d
    material_property_names = 'Gc psic xi c0 l '
    parameter_names = 'p a2 a3 eta '
    parameter_values = '2 -0.5 0 1e-6'
  []
  [nodeg]
    type = NoDegradation
    phase_field = d
    property_name = nodeg
  []
  [coalescence]
    type = ADParsedMaterial
    property_name = coalescence
    material_property_names = 'effective_plastic_strain'
    constant_names = 'beta ep0'
    constant_expressions = '${beta} ${ep0}'
    expression = 1-(1-beta)*(1-exp(-(effective_plastic_strain/ep0)))
    outputs = exodus
    output_properties = 'coalescence'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    property_name = alpha
    expression = 'd'
    phase_field = d
  []
  [defgrad]
    type = ComputeDeformationGradient
  []
  [hencky]
    type = HenckyIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    output_properties = 'psie_active'
    outputs = exodus
  []
  [JohnsonCookHardening]
    type = JohnsonCookHardening
    T = T
    taylor_quinney_factor = 0.9
    sigma_0 = 1
    T0 = 293
    reference_plastic_strain = 1
    reference_plastic_strain_rate = 1.6e-4
    phase_field = d
    degradation_function = nodeg
    A = 517
    B = 405
    C = 0.0075
    n = 0.41
    m = 1.1
    Tm = 750
    output_properties = 'psip_active'
    outputs = exodus
  []
  [J2]
    type = LargeDeformationJ2Plasticity
    hardening_model = JohnsonCookHardening
    relative_tolerance = 1e-08
    output_properties = 'effective_plastic_strain trial'
    outputs = exodus
  []
  [stress]
    type = ComputeLargeDeformationStress
    elasticity_model = hencky
    plasticity_model = J2
  []
[]

[BCs]
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = 'left'
    value = 0
  []
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  []
  [zfix]
    type = DirichletBC
    variable = disp_z
    boundary = 'back'
    value = 0
  []
  [ydisp]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'top'
    function = '${rate}*t'
    preset = false
  []
[]

[Postprocessors]
  [F]
    type = ElementAverageValue
    variable = F
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [stress]
    type = ElementAverageValue
    variable = stress
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [d]
    type = ElementAverageValue
    variable = d
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [ep]
    type = ADElementAverageMaterialProperty
    mat_prop = effective_plastic_strain
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [heat]
    type = ADElementAverageMaterialProperty
    mat_prop = plastic_heat_generation
    execute_on = 'TIMESTEP_END'
  []
  [T]
    type = ElementAverageValue
    variable = T
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  nl_rel_tol = 1e-08
  nl_abs_tol = 1e-10
  nl_max_its = 50

  dt = '${fparse 0.00005 * a / rate}'
  end_time = 4.8e-6

  automatic_scaling = true

  fixed_point_max_its = 100
  fixed_point_rel_tol = 1e-08
  fixed_point_abs_tol = 1e-10
  accept_on_max_fixed_point_iteration = true
  abort_on_solve_fail = true
[]

[Outputs]
  file_base = 'stress_deformation_rate_${rate}_tqf_${tqf}'
  print_linear_residuals = false
  csv = true
  exodus = true
[]

a = 250
ratio = 4

psic = 15
Gc = 1.38e5
l = '${fparse ratio * a}'

E = 6.88e4
nu = 0.33
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

sigma_0 = 2e-7
Q = '${fparse 1.4054e8}'
R = 8.3143e3
T = 800
eps = '${fparse E/100}'

n = 160
rate = 10000
creep_coef = 1

rho = 1
cv = 1
k = 1

tqf = 1

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
    from_multi_app = 'fracture'
    variable = d
    source_variable = d
  []
  [to_fracture]
    type = MultiAppCopyTransfer
    to_multi_app = 'fracture'
    variable = 'psie_active psip_active'
    source_variable = 'psie_active psip_active'
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
    initial_condition = ${T}
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
    prop_names = 'K G l Gc psic sigma_0 Q'
    prop_values = '${K} ${G} ${l} ${Gc} ${psic} ${sigma_0} ${Q}'
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
  [crack_geometric]
    type = CrackGeometricFunction
    property_name = alpha
    expression = 'd'
    phase_field = d
  []
  [arrhenius_law]
    type = ArrheniusLaw
    arrhenius_coefficient = A
    activation_energy = Q
    ideal_gas_constant = ${R}
    temperature = T
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
  [arrhenius_law_hardening]
    type = ArrheniusLawHardening
    reference_stress = sigma_0
    arrhenius_coefficient = A
    eps = ${eps}
    phase_field = d
    degradation_function = g
    taylor_quinney_factor = ${tqf}
    temperature = T
    output_properties = 'psip_active'
    outputs = exodus
  []
  [J2]
    type = LargeDeformationJ2PowerLawCreep
    hardening_model = arrhenius_law_hardening
    coefficient = ${creep_coef}
    exponent = ${n}
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

  dt = '${fparse 0.0005 * a / rate}'
  end_time = '${fparse 0.1 * a / rate}'

  automatic_scaling = true

  fixed_point_max_its = 100
  fixed_point_rel_tol = 1e-08
  fixed_point_abs_tol = 1e-10
  accept_on_max_fixed_point_iteration = true
  abort_on_solve_fail = true
[]

[Outputs]
  file_base = 'stress_deformation_n_${n}_rate_${rate}_tqf_${tqf}'
  print_linear_residuals = false
  csv = true
  exodus = true
[]

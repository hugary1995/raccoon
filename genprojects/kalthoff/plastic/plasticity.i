Gc = 22.2
l = 0.35
psic = 7.9
E = 1.9e5
nu = 0.3
rho = 8e-9
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'
eta = 1
sigma_y = 345 #Check if this value makes sense
n = 1 #?
ep0 = 0.345 #?

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    cli_args = 'Gc=${Gc};l=${l};psic=${psic}'
  []
[]

[Transfers]
  [from_d]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = d
    variable = d
  []
  [to_we_active]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = we_active
    variable = we_active
  []
  [to_wp_active]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = wp_active
    variable = wp_active
  []
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = '../gold/half_notched_plate_63.msh'
  []
[]

[Variables]
  [disp_x]
    block = 1
  []
  [disp_y]
    block = 1
  []
[]

[AuxVariables]
  [stress]
    order = CONSTANT
    family = MONOMIAL
  []
  [d]
  []
[]

[AuxKernels]
  [stress]
    type = ADRankTwoScalarAux
    variable = 'stress'
    rank_two_tensor = 'stress'
    scalar_type = 'MaxPrincipal'
    execute_on = 'TIMESTEP_END'
  []
[]

[Kernels]
  [inertia_x]
    type = InertialForce
    variable = 'disp_x'
    density = 'reg_density'
  []
  [inertia_y]
    type = InertialForce
    variable = 'disp_y'
    density = 'reg_density'
  []
  [solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    component = 0
    displacements = 'disp_x disp_y'
    block = 1
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
    displacements = 'disp_x disp_y'
    block = 1
  []
[]

[BCs]
  [xdisp]
    type = FunctionDirichletBC
    variable = 'disp_x'
    boundary = 'load'
    function = 'if(t<1e-6, 0.5*1.65e10*t*t, 1.65e4*t-0.5*1.65e-2)'
    preset = false
  []
  [y_bot]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'bottom'
    value = '0'
  []
[]

[Materials]
  [fracture_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G l Gc psic viscosity density'
    prop_values = '${K} ${G} ${l} ${Gc} ${psic} ${eta} ${rho}'
    block = 1
  []
  [degradation]
    type = ADDerivativeParsedMaterial
    f_name = g
    args = d
    function = '(1-d)^2/(1+(0.5*Gc/c0/l/psic-1)*d)^2*(1-eta)+eta'
    material_property_names = 'Gc c0 l psic'
    constant_names = 'eta '
    constant_expressions = '5e-3'
    derivative_order = 1
    block = 1
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    phase_field = d
    block = 1
  []
  [defgrad] #?
    type = ComputeDeformationGradient
    displacements = 'disp_x disp_y'
    block = 1
  []
  [hencky] #Look up type of elasicity
    type = HenckyIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    output_properties = 'we_active'
    outputs = exodus
    block = 1
  []
  [power_law_hardening] #Why power law
    type = PowerLawHardening
    yield_stress = ${sigma_y}
    exponent = ${n}
    reference_plastic_strain = ${ep0}
    phase_field = d
    degradation_function = g
    output_properties = 'wp_active'
    outputs = exodus
    block = 1
  []
  [J2]
    type = LargeDeformationJ2Plasticity
    hardening_model = power_law_hardening
    output_properties = 'effective_plastic_strain'
    outputs = exodus
    block = 1
  []
  [newtonian_viscosity]
    type = LargeDeformationNewtonianViscosity
    block = 1
  []
  [stress]
    type = ComputeLargeDeformationStress
    elasticity_model = hencky
    plasticity_model = J2
    viscoelasticity_model = newtonian_viscosity
    block = 1
  []
  [reg_density]
    type = MaterialConverter
    ad_props_in = 'density'
    reg_props_out = 'reg_density'
  []
[]

[Executioner]
  type = Transient
  dt = 5e-9
  end_time = 9e-5

  [TimeIntegrator]
    type = CentralDifference
    solve_type = lumped
  []
[]

[Outputs]
  file_base = 'visualize_explicit'
  exodus = true
  interval = 20
[]

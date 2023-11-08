# Fluid viscosity
eta = 1e-12

# Intrinsic permeability
P = 3e-12

# Fluid source
Q = 4e-3

# Intact Biot coefficient
alpha = 0.79

# Fracture mobility exponent
eps = 50

# Biot modulus
B = 12.5

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [generated_mesh]
    type = GeneratedMeshGenerator
    dim = 2
    elem_type = QUAD4
    nx = ${domain_size}
    ny = ${domain_size}
    nz = 0
    xmin = 0
    xmax = ${domain_size}
    ymin = 0
    ymax = ${domain_size}
    zmin = 0
    zmax = 0
  []
[]

[ICs]
  [d0]
    type = CohesiveDamageIC
    variable = d0
    d0 = 1.0
    l = ${l}
    x1 = '${fparse 0.5*(domain_size-crack_size)}'
    y1 = '${fparse 0.5*domain_size}'
    z1 = 0.0
    x2 = '${fparse 0.5*(domain_size+crack_size)}'
    y2 = '${fparse 0.5*domain_size}'
    z2 = 0
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [pressure]
  []
[]

[AuxVariables]
  [d0]
  []
  [d]
  []
  [psie_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_h]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_xx]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_yy]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_xy]
    order = CONSTANT
    family = MONOMIAL
  []
  [lambda_xx]
    order = CONSTANT
    family = MONOMIAL
  []
  [lambda_xy]
    order = CONSTANT
    family = MONOMIAL
  []
  [lambda_yy]
    order = CONSTANT
    family = MONOMIAL
  []
  [crack_opening]
    order = CONSTANT
    family = MONOMIAL
  []
  [fluid_source_term]
    order = CONSTANT
    family = MONOMIAL
  []
  [biot_coefficient]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[BCs]
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = 'top bottom left right'
    value = 0
  []
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = 'top bottom left right'
    value = 0
  []
  [p]
    type = DirichletBC
    variable = pressure
    boundary = 'top bottom left right'
    value = 0
  []
[]

[Kernels]
  # solid mechaincs
  ###################################
  [solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    displacements = 'disp_x disp_y'
    component = 0
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    displacements = 'disp_x disp_y'
    component = 1
  []
  [poro_x]
    type = ADPoroMechanicsCoupling
    variable = disp_x
    porepressure = pressure
    biot_coefficient = alpha
    component = 0
  []
  [poro_y]
    type = ADPoroMechanicsCoupling
    variable = disp_y
    porepressure = pressure
    biot_coefficient = alpha
    component = 1
  []

  # fluid pressure
  ###################################
  [timederivative_pressure]
    type = ADSPFPressureTimeDerivative
    variable = pressure
    biot_modulus = B
  []
  [timederivative_epsV]
    type = ADSPFVolStrTimeDerivative
    variable = pressure
    biot_coefficient = alpha
  []
  [diffusion_pressure]
    type = ADAnisotropicDiffusion
    variable = pressure
    diffusivity = Mf
  []
  [source_term]
    type = ADCoefMatSource
    variable = pressure
    prop_names = 'Q'
    coefficient = -1
  []
[]

[AuxKernels]
  [E_el]
    type = ADMaterialRealAux
    variable = 'psie_active'
    property = 'psie_active'
    execute_on = 'TIMESTEP_END'
  []
  [stress_h]
    type = ADRankTwoScalarAux
    variable = 'stress_h'
    rank_two_tensor = 'stress'
    scalar_type = Hydrostatic
    execute_on = 'TIMESTEP_END'
  []
  [stress_xx]
    type = ADRankTwoAux
    variable = 'stress_xx'
    rank_two_tensor = 'stress'
    index_i = 0
    index_j = 0
    execute_on = 'TIMESTEP_END'
  []
  [stress_xy]
    type = ADRankTwoAux
    variable = 'stress_xy'
    rank_two_tensor = 'stress'
    index_i = 0
    index_j = 1
    execute_on = 'TIMESTEP_END'
  []
  [stress_yy]
    type = ADRankTwoAux
    variable = 'stress_yy'
    rank_two_tensor = 'stress'
    index_i = 1
    index_j = 1
    execute_on = 'TIMESTEP_END'
  []
  [lambda_xx]
    type = ADMaterialRankTwoTensorAux
    variable = 'lambda_xx'
    property = 'Mf'
    i = 0
    j = 0
    execute_on = 'TIMESTEP_END'
  []
  [lambda_xy]
    type = ADMaterialRankTwoTensorAux
    variable = 'lambda_xy'
    property = 'Mf'
    i = 0
    j = 1
    execute_on = 'TIMESTEP_END'
  []
  [lambda_yy]
    type = ADMaterialRankTwoTensorAux
    variable = 'lambda_yy'
    property = 'Mf'
    i = 1
    j = 1
    execute_on = 'TIMESTEP_END'
  []
  [crack_opening]
    type = ADMaterialRealAux
    variable = 'crack_opening'
    property = 'wn'
    execute_on = 'TIMESTEP_END'
  []
  [fluid_source_term]
    type = ADMaterialRealAux
    variable = 'fluid_source_term'
    property = 'Q'
    execute_on = 'TIMESTEP_END'
  []
  [biot_coefficient]
    type = ADMaterialRealAux
    variable = 'biot_coefficient'
    property = 'alpha'
    execute_on = 'TIMESTEP_END'
  []
[]

[Materials]
  [constants]
    type = ADGenericConstantMaterial
    prop_names = 'eta P'
    prop_values = '${eta} ${P}'
  []
  [biot]
    type = ADGenericConstantMaterial
    prop_names = 'alpha0 B'
    prop_values = '${alpha} ${B}'
  []
  [crack_opening]
    type = ComputeCrackOpeningDisplacement
    phase_field = d
  []
  [mob_w]
    type = ComputeFluidMobility
    fluid_mobility = Mf
    fluid_viscosity = eta
    intrinsic_permeability = P
    phase_field = d
    mobility_exponent = ${eps}
  []
  [fluid_source_term]
    type = ADParsedMaterial
    f_name = Q
    args = 'd0'
    function = 'if(d0>0.5, ${Q}, 0.0)'
  []
  [biot_coefficient]
    type = ADDerivativeParsedMaterial
    f_name = alpha
    material_property_names = 'alpha0'
    args = 'd'
    function = 'alpha0'
    derivative_order = 1
  []

  ## Elasticity ##
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
    parameter_values = '2 1e-3'
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
    decomposition = SPECTRAL
    output_properties = 'elastic_strain psie_active'
    outputs = exodus
  []
  [stress]
    type = ComputeSmallDeformationStress
    elasticity_model = elasticity
    output_properties = 'stress'
    outputs = exodus
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  automatic_scaling = true

  nl_max_its = 50
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8

  dt = 1e-1
  end_time = ${end_time}
[]

[Outputs]
  file_base = results
  exodus = true
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
  print_linear_residuals = false
[]

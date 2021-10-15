eta = 1e-12
k_int = 3e-12
Q = 4e-3
alpha = 0.79
eps = 50
M = 12.5

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

[GlobalParams]
  displacements = 'disp_x disp_y'
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
    variable = 'disp_x'
    boundary = 'top bottom left right'
    value = 0
  []
  [yfix]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'top bottom left right'
    value = 0
  []
  [p]
    type = DirichletBC
    variable = 'pressure'
    boundary = 'top bottom left right'
    value = 0
  []
[]

[Kernels]
  # solid mechaincs
  ###################################
  [solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    displacements = 'disp_x disp_y'
    component = 0
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    displacements = 'disp_x disp_y'
    component = 1
  []
  [poro_x]
    type = ADPoroMechanicsCoupling
    variable = 'disp_x'
    porepressure = 'pressure'
    component = 0
  []
  [poro_y]
    type = ADPoroMechanicsCoupling
    variable = 'disp_y'
    porepressure = 'pressure'
    component = 1
  []

  # pressure
  ###################################
  [timederivative_pressure]
    type = ADSPFPressureTimeDerivative
    variable = pressure
  []
  [timederivative_epsV]
    type = ADSPFVolStrTimeDerivative
    variable = pressure
  []
  [diffusion_pressure]
    type = ADAnisotropicDiffusion
    variable = pressure
    diffusivity = 'fluid_mobility'
  []
  [source_term]
    type = ADCoefMatSource
    variable = pressure
    prop_names = 'fluid_source_term'
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
    property = 'fluid_mobility'
    i = 0
    j = 0
    execute_on = 'TIMESTEP_END'
  []
  [lambda_xy]
    type = ADMaterialRankTwoTensorAux
    variable = 'lambda_xy'
    property = 'fluid_mobility'
    i = 0
    j = 1
    execute_on = 'TIMESTEP_END'
  []
  [lambda_yy]
    type = ADMaterialRankTwoTensorAux
    variable = 'lambda_yy'
    property = 'fluid_mobility'
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
    property = 'fluid_source_term'
    execute_on = 'TIMESTEP_END'
  []
  [biot_coefficient]
    type = ADMaterialRealAux
    variable = 'biot_coefficient'
    property = 'biot_coefficient'
    execute_on = 'TIMESTEP_END'
  []
[]

[Materials]
  [constants]
    type = ADGenericConstantMaterial
    prop_names = 'fluid_viscosity intrinsic_permeability fluid_source'
    prop_values = '${eta} ${k_int} ${Q}'
  []
  [biot]
    type = ADGenericConstantMaterial
    prop_names = 'intact_biot_coef biot_modulus'
    prop_values = '${alpha} ${M}'
  []
  [crack_opening]
    type = ComputeCrackOpeningDisplacement
    phase_field = d
  []
  [mob_w]
    type = ADComputeFluidMobility
    damage = 'd'
    mobility_exponent = '${eps}'
  []
  [fluid_source_term]
    type = ADParsedMaterial
    f_name = fluid_source_term
    material_property_names = 'fluid_source'
    args = 'd0'
    function = 'if(d0>0.5, fluid_source, 0.0)'
  []
  [biot_coefficient]
    type = ADDerivativeParsedMaterial
    f_name = biot_coefficient
    material_property_names = 'intact_biot_coef'
    args = 'd'
    function = 'intact_biot_coef'
    enable_jit = true
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
  nl_max_its = 50
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8
  dt = 1e-1
  end_time = ${end_time}
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  automatic_scaling = true
[]

[Outputs]
  file_base = results
  exodus = true
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
  print_linear_residuals = false
[]

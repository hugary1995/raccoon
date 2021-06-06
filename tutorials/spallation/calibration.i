# ideal gas constant
R = 8.3143

# oxide parameters
E_oxide = 1.2e5
nu_oxide = 0.24
K_oxide = '${fparse E_oxide/3/(1-2*nu_oxide)}'
G_oxide = '${fparse E_oxide/2/(1+nu_oxide)}'
kappa_oxide = 3
Q_oxide = 1.4054e2
# Q_oxide = 4.2162e8
sigma0_oxide = '${fparse 211*7}'
creep_coef_oxide = 8.5875e-8
# creep_coef_oxide = 9.14
n_oxide = 3

# metal parameters
E_metal = 1.9e5
nu_metal = 0.3
K_metal = '${fparse E_metal/3/(1-2*nu_metal)}'
G_metal = '${fparse E_metal/2/(1+nu_metal)}'
kappa_metal = 30
Q_metal = 7.91e1
# Q_metal = 4e8
sigma0_metal = '${fparse 294*7}'
creep_coef_metal = 2.3e-9
n_metal = 5.06

# boundary condition
h_gas = 0.1
h_steam = 2.8

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[MultiApps]
  [ref_temp]
    type = FullSolveMultiApp
    input_files = 'ref_temp.i'
    execute_on = 'INITIAL'
  []
[]

[Transfers]
  [from_temp]
    type = MultiAppMeshFunctionTransfer
    direction = from_multiapp
    multi_app = ref_temp
    source_variable = temp
    variable = ref_temp
    execute_on = 'INITIAL'
  []
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = gold/cylinder.msh
  []
[]

[Functions]
  [CTE_oxide]
    type = PiecewiseLinear
    format = columns
    data_file = 'gold/CTE_oxide.csv'
  []
  [CTE_metal]
    type = PiecewiseLinear
    format = columns
    data_file = 'gold/CTE_metal.csv'
  []
  [T_gas]
    type = ParsedFunction
    value = 'if(t<1, 1100+(845-1100)*t, 845+(25-845)*(t-1))'
  []
  [T_steam]
    type = ParsedFunction
    value = 'if(t<1, 530+(525-530)*t, 525+(25-525)*(t-1))'
  []
  [p_gas]
    type = ParsedFunction
    value = 'if(t<1, 0.2+(0.1-0.2)*t, 0.1)'
  []
  [p_steam]
    type = ParsedFunction
    value = 'if(t<1, 17+(10-17)*t, 10+(0.1-10)*(t-1))'
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [disp_z]
  []
  [temp]
  []
  [strain_zz]
    block = oxide_lower_dimensional_block
  []
[]

[AuxVariables]
  [d]
  []
  [ref_temp]
  []
  [temp_in_K]
  []
  [stress_xx]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_yy]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_zz]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [temp_in_K]
    type = ParsedAux
    variable = 'temp_in_K'
    args = 'temp'
    function = 'temp + 273.15'
  []
  [stress_xx]
    type = ADRankTwoAux
    variable = 'stress_xx'
    rank_two_tensor = 'stress'
    index_i = 0
    index_j = 0
  []
  [stress_yy]
    type = ADRankTwoAux
    variable = 'stress_yy'
    rank_two_tensor = 'stress'
    index_i = 1
    index_j = 1
  []
  [stress_zz]
    type = ADRankTwoAux
    variable = 'stress_zz'
    rank_two_tensor = 'stress'
    index_i = 2
    index_j = 2
  []
[]

[Kernels]
  [conduction]
    type = HeatConduction
    variable = 'temp'
  []
  # metal
  [solid_x_metal]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    use_displaced_mesh = true
    block = metal
  []
  [solid_y_metal]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    use_displaced_mesh = true
    block = metal
  []
  [solid_z_metal]
    type = ADStressDivergenceTensors
    variable = disp_z
    component = 2
    use_displaced_mesh = true
    block = metal
  []
  # oxide
  [solid_x_oxide]
    type = ADLowerDimensionalStressDivergenceTensors
    variable = disp_x
    component = 0
    thickness = thickness
    use_displaced_mesh = true
    block = oxide_lower_dimensional_block
  []
  [solid_y_oxide]
    type = ADLowerDimensionalStressDivergenceTensors
    variable = disp_y
    component = 1
    thickness = thickness
    use_displaced_mesh = true
    block = oxide_lower_dimensional_block
  []
  [solid_z_oxide]
    type = ADLowerDimensionalStressDivergenceTensors
    variable = disp_z
    component = 2
    thickness = thickness
    use_displaced_mesh = true
    block = oxide_lower_dimensional_block
  []
  [pressure]
    type = ADLowerDimensionalWeakOutOfPlaneStress
    variable = strain_zz
    out_of_plane_stress = p_steam
    block = oxide_lower_dimensional_block
  []
  [react_x]
    type = ADCoefMatReaction
    variable = disp_x
    coefficient = '${fparse E_oxide/50}'
    prop_names = 'g'
    block = oxide_lower_dimensional_block
  []
  [react_y]
    type = ADCoefMatReaction
    variable = disp_y
    coefficient = '${fparse E_oxide/50}'
    prop_names = 'g'
    block = oxide_lower_dimensional_block
  []
  [react_z]
    type = ADCoefMatReaction
    variable = disp_z
    coefficient = '${fparse E_oxide/50}'
    prop_names = 'g'
    block = oxide_lower_dimensional_block
  []
[]

[BCs]
  [fix_bottom_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'bottom'
    value = 0
  []
  [fix_left_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'left'
    value = 0
  []
  [fix_right_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'right'
    value = 0
  []
  [convection_left]
    type = ConvectiveHeatFluxBC
    variable = temp
    boundary = 'inner'
    T_infinity = T_steam
    heat_transfer_coefficient = ${h_steam}
  []
  [convection_right]
    type = ConvectiveHeatFluxBC
    variable = temp
    boundary = 'outer'
    T_infinity = T_gas
    heat_transfer_coefficient = ${h_gas}
  []
  [Pressure]
    [inner]
      boundary = 'inner'
      function = p_steam
      use_automatic_differentiation = true
    []
    [outer]
      boundary = 'outer'
      function = p_gas
      use_automatic_differentiation = true
    []
  []
[]

[Materials]
  [environment]
    type = GenericFunctionMaterial
    prop_names = 'T_gas T_steam'
    prop_values = 'T_gas T_steam'
    constant_on = SUBDOMAIN
  []
  # metal elasticity
  [degradation_metal]
    type = NoDegradation
    f_name = g
    phase_field = d
    block = metal
  []
  [metal_thermal_properties]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '${kappa_metal}'
    block = metal
  []
  [metal_elastic_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G Q sigma0'
    prop_values = '${K_metal} ${G_metal} ${Q_metal} ${sigma0_metal}'
    block = metal
  []
  [eigenstrain_metal]
    type = ComputeThermalExpansionEigenDeformationGradient
    reference_temperature = ref_temp
    temperature = temp
    thermal_expansion_function = CTE_oxide
    eigen_deformation_gradient_name = thermal_defgrad
    block = metal
  []
  [deformation_gradient_metal]
    type = ComputeDeformationGradient
    eigen_deformation_gradient_names = 'thermal_defgrad'
    block = metal
  []
  [arrhenius_law_metal]
    type = ArrheniusLaw
    arrhenius_coefficient = A
    activation_energy = ${Q_metal}
    ideal_gas_constant = ${R}
    T = temp_in_K
    block = metal
  []
  [stress_metal]
    type = ComputeLargeDeformationXMATStress
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    strain_energy_density = psie
    coefficient = ${creep_coef_metal}
    exponent = ${n_metal}
    reference_stress = sigma0
    arrhenius_coefficient = A
    eps = '${fparse E_metal/100}'
    plastic_energy_density = psip
    plastic_degradation_function = g
    block = metal
  []

  # oxide elasticity
  [degradation_oxide]
    type = NoDegradation
    f_name = g
    phase_field = d
    block = oxide_lower_dimensional_block
  []
  [oxide_thermal_properties]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '${kappa_oxide}'
    block = oxide_lower_dimensional_block
  []
  [oxide_elastic_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G Q sigma0'
    prop_values = '${K_oxide} ${G_oxide} ${Q_oxide} ${sigma0_oxide}'
    block = oxide_lower_dimensional_block
  []
  [oxide_thickness]
    type = GenericConstantMaterial
    prop_names = 'thickness'
    prop_values = '0.2'
    block = oxide_lower_dimensional_block
  []
  [eigenstrain_oxide]
    type = ComputeThermalExpansionEigenDeformationGradient
    reference_temperature = ref_temp
    temperature = temp
    thermal_expansion_function = CTE_metal
    eigen_deformation_gradient_name = thermal_defgrad
    block = oxide_lower_dimensional_block
  []
  [deformation_gradient_oxide]
    type = ComputeLowerDimensionalDeformationGradient
    eigen_deformation_gradient_names = 'thermal_defgrad'
    out_of_plane_strain = strain_zz
    block = oxide_lower_dimensional_block
  []
  [arrhenius_law_oxide]
    type = ArrheniusLaw
    arrhenius_coefficient = A
    activation_energy = ${Q_oxide}
    ideal_gas_constant = ${R}
    T = temp_in_K
    block = oxide_lower_dimensional_block
  []
  [stress_oxide]
    type = ComputeLargeDeformationXMATStress
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    strain_energy_density = psie
    coefficient = ${creep_coef_oxide}
    exponent = ${n_oxide}
    reference_stress = sigma0
    arrhenius_coefficient = A
    eps = '${fparse E_oxide/100}'
    plastic_energy_density = psip
    degradation_function = g
    plastic_degradation_function = g
    block = oxide_lower_dimensional_block
  []
[]

[Postprocessors]
  [stress_rr]
    type = PointValue
    variable = stress_xx
    point = '14 0 10'
  []
  [stress_tt]
    type = PointValue
    variable = stress_yy
    point = '14 0 10'
  []
  [stress_zz]
    type = PointValue
    variable = stress_zz
    point = '14 0 10'
  []
  [psie_active_avg]
    type = ADElementAverageMaterialProperty
    mat_prop = psie_active
    block = oxide_lower_dimensional_block
  []
  [psip_active_avg]
    type = ADElementAverageMaterialProperty
    mat_prop = psip_active
    block = oxide_lower_dimensional_block
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'

  dt = 0.1
  end_time = 2

  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08

  automatic_scaling = true

  [Quadrature]
    order = CONSTANT
  []
[]

[Outputs]
  print_linear_residuals = false
  exodus = true
  csv = true
[]

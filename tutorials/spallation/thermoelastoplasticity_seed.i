mesh = 'gold/cylinder3.msh'

# ideal gas constant
R = 8.3143

# oxide parameters
E_oxide = 1.2e5
nu_oxide = 0.3
K_oxide = '${fparse E_oxide/3/(1-2*nu_oxide)}'
G_oxide = '${fparse E_oxide/2/(1+nu_oxide)}'
kappa_oxide = 3
Q_oxide = 210
sigma0_oxide = '${fparse 290}'
creep_coef_oxide = 0
n_oxide = 10

# metal parameters
E_metal = 1.9e5
nu_metal = 0.3
K_metal = '${fparse E_metal/3/(1-2*nu_metal)}'
G_metal = '${fparse E_metal/2/(1+nu_metal)}'
kappa_metal = 30
Q_metal = 80
sigma0_metal = '${fparse 345}'
creep_coef_metal = 1e-7
n_metal = 5

# boundary condition
h_gas = 0.1
h_steam = 2.8

# fracture properties
# psic = 0.013
# psic = 0.0065
psic = 0.0125
Gc = 0.1
l = 0.5
ep0 = 1e-6
beta = 0.05

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[MultiApps]
  [ref_temp]
    type = FullSolveMultiApp
    input_files = 'ref_temp.i'
    execute_on = 'INITIAL'
    cli_args = 'mesh=${mesh}'
  []
  [fracture]
    type = TransientMultiApp
    input_files = 'fracture_seed.i'
    cli_args = 'psic=${psic};l=${l};ep0=${ep0};beta=${beta};Gc=${Gc};mesh=${mesh}'
    execute_on = 'INITIAL TIMESTEP_END'
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
  [from_d]
    type = MultiAppMeshFunctionTransfer
    multi_app = fracture
    direction = from_multiapp
    variable = d
    source_variable = d
  []
  [to_psie_active]
    type = MultiAppMeshFunctionTransfer
    multi_app = fracture
    direction = to_multiapp
    variable = psie_active
    source_variable = psie_active
  []
  [to_ep_active]
    type = MultiAppMeshFunctionTransfer
    multi_app = fracture
    direction = to_multiapp
    variable = effective_creep_strain
    source_variable = effective_creep_strain
  []
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = ${mesh}
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
    type = VectorPostprocessorFunction
    vectorpostprocessor_name = 'bcs'
    argument_column = 'time'
    value_column = 'T_gas'
  []
  [T_steam]
    type = VectorPostprocessorFunction
    vectorpostprocessor_name = 'bcs'
    argument_column = 'time'
    value_column = 'T_steam'
  []
  [p_gas]
    type = VectorPostprocessorFunction
    vectorpostprocessor_name = 'bcs'
    argument_column = 'time'
    value_column = 'p_gas'
  []
  [p_steam]
    type = VectorPostprocessorFunction
    vectorpostprocessor_name = 'bcs'
    argument_column = 'time'
    value_column = 'p_steam'
  []
  [dt]
    type = VectorPostprocessorFunction
    vectorpostprocessor_name = 'bcs'
    argument_column = 'time'
    value_column = 'dt'
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
[]

[AuxVariables]
  [d]
    block = oxide
  []
  [c]
    order = CONSTANT
    family = MONOMIAL
    block = oxide
  []
  [ref_temp]
  []
  [temp_in_K]
  []
  [effective_creep_strain]
    order = CONSTANT
    family = MONOMIAL
  []
  [psie_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [psii_active]
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
  [c]
    type = MaterialRealAux
    variable = c
    property = debonding_indicator
    block = oxide
  []
  [effective_creep_strain]
    type = MaterialRealAux
    variable = effective_creep_strain
    property = effective_creep_strain
    block = oxide
  []
  [psie_active]
    type = MaterialRealAux
    variable = psie_active
    property = psie_active
    block = oxide
  []
  [psii_active]
    type = MaterialRealAux
    variable = psii_active
    property = psii_active
    block = oxide
  []
[]

[Kernels]
  [conduction]
    type = HeatConduction
    variable = temp
  []
  # metal
  [solid_x_metal]
    type = StressDivergenceTensors
    variable = disp_x
    component = 0
    use_displaced_mesh = true
    temperature = temp
    eigenstrain_names = 'thermal_eigenstrain'
    block = metal
  []
  [solid_y_metal]
    type = StressDivergenceTensors
    variable = disp_y
    component = 1
    use_displaced_mesh = true
    temperature = temp
    eigenstrain_names = 'thermal_eigenstrain'
    block = metal
  []
  [solid_z_metal]
    type = StressDivergenceTensors
    variable = disp_z
    component = 2
    use_displaced_mesh = true
    temperature = temp
    eigenstrain_names = 'thermal_eigenstrain'
    block = metal
  []
  # oxide
  [solid_x_oxide]
    type = LowerDimensionalStressDivergenceTensors
    variable = disp_x
    component = 0
    use_displaced_mesh = true
    temperature = temp
    eigenstrain_names = 'thermal_eigenstrain'
    thickness = thickness
    block = oxide
  []
  [solid_y_oxide]
    type = LowerDimensionalStressDivergenceTensors
    variable = disp_y
    component = 1
    use_displaced_mesh = true
    temperature = temp
    eigenstrain_names = 'thermal_eigenstrain'
    thickness = thickness
    block = oxide
  []
  [solid_z_oxide]
    type = LowerDimensionalStressDivergenceTensors
    variable = disp_z
    component = 2
    use_displaced_mesh = true
    temperature = temp
    eigenstrain_names = 'thermal_eigenstrain'
    thickness = thickness
    block = oxide
  []
[]

[BCs]
  [fix_bottom_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'top bottom'
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
      use_automatic_differentiation = false
    []
    [outer]
      boundary = 'outer'
      function = p_gas
      use_automatic_differentiation = false
    []
  []
[]

# [Constraints]
#   [top]
#     type = EqualValueBoundaryConstraint
#     variable = disp_z
#     penalty = 1e9
#     secondary = top
#   []
# []

[Dampers]
  [ejd]
    type = ElementJacobianDamper
    max_increment = 0.01
    min_damping = 0
    use_displaced_mesh = true
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
  [thermal_metal]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '${kappa_metal}'
    block = metal
  []
  [elasticity_tensor_metal]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = ${E_metal}
    poissons_ratio = ${nu_metal}
    block = metal
  []
  [eigenstrain_metal]
    type = ComputeInstantaneousThermalExpansionFunctionEigenstrain
    stress_free_temperature = ref_temp
    temperature = temp
    thermal_expansion_function = CTE_metal
    eigenstrain_name = 'thermal_eigenstrain'
    block = metal
  []
  [strain_metal]
    type = ComputeFiniteStrain
    eigenstrain_names = 'thermal_eigenstrain'
    block = metal
  []
  [creep_metal]
    type = PowerLawCreepStressUpdate
    coefficient = 2.3e6
    n_exponent = 5.06
    activation_energy = 4e8
    gas_constant = 8.3143e3
    temperature = 'temp_in_K'
    block = metal
  []
  [stress_metal]
    type = ComputeMultipleInelasticStress
    inelastic_models = 'creep_metal'
    block = metal
  []

  # oxide elasticity
  [fracture_properties]
    type = GenericConstantMaterial
    prop_names = 'l Gc psic'
    prop_values = '${l} ${Gc} ${psic}'
    block = oxide
  []
  [gc]
    type = ParsedMaterial
    f_name = gc
    args = effective_creep_strain
    function = '1-(1-beta)*(1-exp(-effective_creep_strain/ep0))'
    constant_names = 'beta ep0'
    constant_expressions = '${beta} ${ep0}'
    block = oxide
    output_properties = 'gc'
    outputs = exodus
  []
  [in_plane_degradation]
    type = ParsedMaterial
    f_name = gip
    args = d
    function = (1-d)^p/((1-d)^p+(Gc/psic*xi/c0/l)*d*(1+a2*d+a2*a3*d^2))*(1-eta)+eta
    constant_names = 'p xi c0 a2 a3 eta'
    constant_expressions = '2 1 ${fparse 8/3} 1 0 1e-6'
    material_property_names = 'Gc psic l'
    block = oxide
  []
  [out_of_plane_fracture_toughness]
    type = ParsedMaterial
    f_name = gamma
    function = 'Gc/thickness/0.2/1e3'
    material_property_names = 'Gc thickness'
    block = oxide
  []
  [thickness]
    type = GenericFunctionMaterial
    prop_names = 'thickness'
    prop_values = '1.468e-5*sqrt(t)/0.2*100'
    block = oxide
  []
  [thermal_oxide]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '${kappa_oxide}'
    block = oxide
  []
  [eigenstrain_oxide]
    type = ComputeInstantaneousThermalExpansionFunctionEigenstrain
    stress_free_temperature = ref_temp
    temperature = temp
    thermal_expansion_function = CTE_oxide
    eigenstrain_name = 'thermal_eigenstrain'
    block = oxide
  []
  [elasticity_tensor_oxide]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = ${E_oxide}
    poissons_ratio = ${nu_oxide}
    block = oxide
  []
  [strain_oxide]
    type = ComputeFiniteStrain
    eigenstrain_names = 'thermal_eigenstrain'
    block = oxide
  []
  [creep_oxide]
    type = PowerLawCreepStressUpdate
    coefficient = 8.5875e7
    n_exponent = 3
    activation_energy = 4.2162e8
    gas_constant = 8.3143e3
    temperature = 'temp_in_K'
    block = oxide
  []
  [stress_oxide]
    type = PFFComputeMultipleInelasticStress
    inelastic_models = 'creep_oxide'
    in_plane_degradation_function = gip
    out_of_plane_fracture_toughness = gamma
    out_of_plane_critical_fracture_energy = 5e-7
    block = oxide
    outputs = exodus
    output_properties = 'elastic_strain stress'
  []
[]

[Postprocessors]
  [years]
    type = FunctionValuePostprocessor
    function = 't/86400/360'
  []
  [psie]
    type = ElementIntegralMaterialProperty
    mat_prop = psie
    block = oxide
  []
  [psii]
    type = ElementIntegralMaterialProperty
    mat_prop = psii
    block = oxide
  []
  # [psie_active]
  #   type = ElementIntegralMaterialProperty
  #   mat_prop = psie_active
  #   block = oxide
  # []
  # [psii_active]
  #   type = ElementIntegralMaterialProperty
  #   mat_prop = psii_active
  #   block = oxide
  # []
  [solution_change]
    type = SolutionChangeNorm
    variable = 'disp_x disp_y disp_z d'
    block = oxide
    outputs = none
  []
[]

[VectorPostprocessors]
  [bcs]
    type = CSVReader
    csv_file = 'gold/BC_seed.csv'
    header = true
  []
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Transient
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       strumpack                    '
  # petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  # petsc_options_value = 'lu       superlu_dist                 '
  # petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels '
  # petsc_options_value = 'asm      ilu          200         200                0                     '
  [TimeStepper]
    type = CSVTimeSequenceStepper
    file_name = 'gold/BC_seed.csv'
    header = true
  []
  end_time = 15674400

  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08

  automatic_scaling = true

  picard_max_its = 50
  picard_custom_pp = solution_change
  custom_rel_tol = 1e-03
  custom_abs_tol = 1e-03
  disable_picard_residual_norm_check = true
  accept_on_max_picard_iteration = true
[]

[Outputs]
  print_linear_residuals = true
  [exodus]
    type = Exodus
    file_base = 'seed_no_fracture'
  []
  csv = true
  [checkpoint]
    type = Checkpoint
    file_base = 'checkpoint'
    num_files = 2
  []
[]
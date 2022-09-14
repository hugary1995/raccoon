a = 1

E = 201.8e3
nu = 0.3
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

T_init = 300

rho = 7.9e-9
cv = 4.47e7
k = 44
alpha = 1

rate = 100

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  volumetric_locking_correction = true
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
[]

[Materials]
  [thermal_properties]
    type = GenericConstantMaterial
    prop_names = 'density specific_heat thermal_conductivity'
    prop_values = '${rho} ${cv} ${k}'
  []
  [bulk]
    type = ADGenericConstantMaterial
    prop_names = 'K G'
    prop_values = '${K} ${G}'
  []
  [no_degradation]
    type = NoDegradation
    f_name = g
    function = 1
    phase_field = d
  []
  [thermal_expansion]
    type = ComputeThermalExpansionEigenDeformationGradient
    eigen_deformation_gradient_name = eig_strain
    reference_temperature = 300
    temperature = T
    thermal_expansion_function = '${alpha}'
  []
  [defgrad]
    type = ComputeDeformationGradient
    eigen_deformation_gradient_names = eig_strain
  []
  [elasticity]
    type = HenckyIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    output_properties = 'elastic_strain'
    outputs = exodus
  []
  [stress]
    type = ComputeLargeDeformationStress
    elasticity_model = elasticity
    output_properties = 'stress'
    outputs = exodus
  []
[]

[BCs]
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = 'left right'
    value = 0
  []
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom top'
    value = 0
  []
  [zfix]
    type = DirichletBC
    variable = disp_z
    boundary = 'back front'
    value = 0
  []
  [heat]
    type = FunctionDirichletBC
    variable = T
    boundary = 'top'
    function = '300+${rate}*t'
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

  dt = '${fparse 0.1/rate}'
  end_time = '${fparse  1/rate}'

  automatic_scaling = true

  abort_on_solve_fail = true
[]

[Outputs]
  file_base = 'stress_alpha_${alpha}'
  print_linear_residuals = false
  csv = true
  exodus = true
[]

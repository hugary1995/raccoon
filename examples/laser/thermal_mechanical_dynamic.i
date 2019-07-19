[Mesh]
  type = FileMesh
  file = gold/geo.msh
[]

[Problem]
  coord_type = RZ
[]

[Variables]
  [./disp_r]
  [../]
  [./disp_z]
  [../]
  [./T]
    initial_condition = 300
  [../]
[]

[AuxVariables]
  [./vel_r]
  [../]
  [./vel_z]
  [../]
  [./accel_r]
  [../]
  [./accel_z]
  [../]
  [./stress]
    family = MONOMIAL
  [../]
  [./d]
  [../]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./E_el]
    type = MaterialRealAux
    property = 'E_el'
    variable = 'E_el'
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Materials]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '8000'
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1e10
    poissons_ratio = 0.345
  [../]
  [./strain]
    type = ADComputeAxisymmetricRZSmallStrain
    displacements = 'disp_r disp_z'
    eigenstrain_names = 'thermal_eigenstrain'
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_StrainSpectral
    d = d
    history = false
  [../]

  [./thermal]
    type = HeatConductionMaterial
    temp = 'T'
    thermal_conductivity = 2
    specific_heat = 1e8
  [../]
  [./thermal_expansion]
    type = ADComputeThermalExpansionEigenstrain
    stress_free_temperature = '300'
    temperature = 'T'
    thermal_expansion_coeff = 8e-6
    eigenstrain_name = thermal_eigenstrain
  [../]

  [./fracture_energy_barrier]
    type = GenericFunctionMaterial
    prop_names = 'b'
    prop_values = '14.88'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = 2.7
    L = 0.015
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-06
  [../]
[]

[AuxKernels]
  [./accel_r]
    type = NewmarkAccelAux
    variable = accel_r
    displacement = disp_r
    velocity = vel_r
    beta = 0.25
    execute_on = timestep_end
  [../]
  [./vel_r]
    type = NewmarkVelAux
    variable = vel_r
    acceleration = accel_r
    gamma = 0.5
    execute_on = timestep_end
  [../]
  [./accel_z]
    type = NewmarkAccelAux
    variable = accel_z
    displacement = disp_z
    velocity = vel_z
    beta = 0.25
    execute_on = timestep_end
  [../]
  [./vel_z]
    type = NewmarkVelAux
    variable = vel_z
    acceleration = accel_z
    gamma = 0.5
    execute_on = timestep_end
  [../]
  [./stress]
    type = RankTwoScalarAux
    variable = stress
    rank_two_tensor = stress
    scalar_type = MaxPrincipal
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Kernels]
  [./inertia_r]
    type = InertialForce
    variable = disp_r
    velocity = vel_r
    acceleration = accel_r
    beta = 0.25
    gamma = 0.5
  [../]
  [./inertia_z]
    type = InertialForce
    variable = disp_z
    velocity = vel_z
    acceleration = accel_z
    beta = 0.25
    gamma = 0.5
  [../]
  [./solid_r]
    type = ADStressDivergenceRZTensors
    variable = disp_r
    component = 0
    displacements = 'disp_r disp_z'
  [../]
  [./solid_z]
    type = ADStressDivergenceRZTensors
    variable = disp_z
    component = 1
    displacements = 'disp_r disp_z'
  [../]

  [./Hcond_time]
    type = ADHeatConductionTimeDerivative
    variable = T
  [../]
  [./Hcond]
    type = ADHeatConduction
    variable = T
  [../]
  [./Hsource]
    type = HeatSource
    variable = T
    function = '100*t'
    block = source
  []
[]

[BCs]
  [./bottom_r_fix]
    type = PresetBC
    variable = disp_r
    boundary = 'bottom'
    value = 0
  [../]
  [./bottom_z_fix]
    type = PresetBC
    variable = disp_z
    boundary = 'bottom'
    value = 0
  [../]
  [./top_z_disp]
    type = FunctionPresetBC
    variable = disp_z
    boundary = 'top'
    function = -t
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'

  dt = 0.1
  end_time = 1

  automatic_scaling = false
[]

[Outputs]
  exodus = true
[]

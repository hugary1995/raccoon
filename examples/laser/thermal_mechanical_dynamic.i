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
  [./bounds_dummy]
  [../]
[]

[Bounds]
  [./T]
    type = BoundsAux
    variable = bounds_dummy
    bounded_variable = T
    lower = 300
    execute_on = 'LINEAR TIMESTEP_BEGIN'
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

[Functions]
  [./Gc]
    type = PiecewiseMultilinear
    data_file = gold/Gc.txt
  [../]
[]

[Materials]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '2.6e-9'
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1.08e4
    poissons_ratio = 0.3461
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
    thermal_conductivity = 2.259
    specific_heat = 8.343e8
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
    prop_values = '0.005'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = Gc
    L = 0.1
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
    function = 'if (t<1e-7, 1e18*t, if (t<9e-7, 1e11, if (t<1e-6, 1e12-1e18*t, 0)))'
    block = source
  []
[]

[BCs]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu       vinewtonrsls'

  dt = 5e-9
  end_time = 1

  automatic_scaling = false
[]

[Outputs]
  exodus = true
[]

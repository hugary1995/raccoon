[Mesh]
  type = FileMesh
  file = ../gold/geo2.msh
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
  [./Gc]
  [../]
[]

[Postprocessors]
  [./thermal_ablation_volume]
    type = ThermalAblationVolume
    d = d
    temperature = T
    ablation_threshold = 1612
  [../]

  [./d_integral]
    type = ElementIntegralVariablePostprocessor
    variable = d
  [../]
[]

[AuxKernels]
  [./Gc]
    type = FunctionAux
    variable = Gc
    function = Gc
    execute_on = 'INITIAL'
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
    data_file = ../gold/Gc2.txt
  [../]
[]

[Materials]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '2.0e-9'
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.74e4
    poissons_ratio = 0.25
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
    prop_values = '0.0009'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = Gc
    L = 0.18
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
    function = 'if (t<1.000000E-06, 3.418703E+13*t, if (t<2.085000E-04, 3.418703E+07, if (t<2.095000E-04, 7.162184E+09-3.418703E+13*t, 0)))'
    block = source
  []
[]

[BCs]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type'
  petsc_options_value = 'asm      ilu          200         200                0                     vinewtonrsls'

  dt = 5e-9
  end_time = 2.095000E-04

  automatic_scaling = false
[]

[Outputs]
  file_base = run6
  print_linear_residuals = false
  exodus = true
  csv = true
[]

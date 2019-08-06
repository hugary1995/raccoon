[GlobalParams]
  beta = 0.25
  gamma = 0.5
  dt_master = 5e-9
[]

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
  [./load]
    family = SCALAR
  [../]

  [./disp_r_ref]
  [../]
  [./disp_z_ref]
  [../]
  [./vel_r_ref]
  [../]
  [./vel_z_ref]
  [../]
  [./accel_r_ref]
  [../]
  [./accel_z_ref]
  [../]
[]

[Bounds]
  [./T]
    type = BoundsAux
    variable = bounds_dummy
    bounded_variable = T
    lower = 300
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
    type = InertialForceFPI
    variable = disp_r
    displacement_old = disp_r_ref
    velocity = vel_r_ref
    acceleration = accel_r_ref
  [../]
  [./inertia_z]
    type = InertialForceFPI
    variable = disp_z
    displacement_old = disp_z_ref
    velocity = vel_z_ref
    acceleration = accel_z_ref
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
    type = ScalarBodyForce
    variable = T
    scalar = load
    block = source
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type  '
  petsc_options_value = 'asm      ilu          1000        200                0                     vinewtonrsls'

  dt = 5e-11
  end_time = 1

  nl_abs_tol = 1e-8
  nl_rel_tol = 1e-6

  automatic_scaling = false
[]

[Outputs]
  hide = 'load'
  exodus = true
  print_linear_residuals = false
[]

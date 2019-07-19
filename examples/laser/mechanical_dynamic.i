[Mesh]
  # type = GeneratedMesh
  # dim = 2
  # xmin = 0
  # xmax = 10
  # ymin = 0
  # ymax = 30
  # nx = 10
  # ny = 30
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
  [../]
  [./stress]
    type = ADComputeLinearElasticStress
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
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  dt = 0.1
  end_time = 1

  automatic_scaling = true
[]

[Outputs]
  exodus = true
[]

[GlobalParams]
  beta = 0.25
  gamma = 0.5
  dt_master = 0.05
[]

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 4
  nz = 1
  xmin = 0.0
  xmax = 0.1
  ymin = 0.0
  ymax = 4.0
  zmin = 0.0
  zmax = 0.1
[]


[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
[]

[AuxVariables]
  [./load]
    family = SCALAR
  [../]

  [./disp_x_ref]
  [../]
  [./vel_x_ref]
  [../]
  [./accel_x_ref]
  [../]

  [./disp_y_ref]
  [../]
  [./vel_y_ref]
  [../]
  [./accel_y_ref]
  [../]

  [./disp_z_ref]
  [../]
  [./vel_z_ref]
  [../]
  [./accel_z_ref]
  [../]
[]

[Kernels]
  [./solid_x]
    type = StressDivergenceTensors
    variable = disp_x
    component = 0
    displacements = 'disp_x disp_y disp_z'
  [../]
  [./solid_y]
    type = StressDivergenceTensors
    variable = disp_y
    component = 1
    displacements = 'disp_x disp_y disp_z'
  [../]
  [./solid_z]
    type = StressDivergenceTensors
    variable = disp_z
    component = 2
    displacements = 'disp_x disp_y disp_z'
  [../]
  [./inertia_x]
    type = InertialForceFPI
    variable = disp_x
    displacement_old = disp_x_ref
    velocity = vel_x_ref
    acceleration = accel_x_ref
  [../]
  [./inertia_y]
    type = InertialForceFPI
    variable = disp_y
    displacement_old = disp_y_ref
    velocity = vel_y_ref
    acceleration = accel_y_ref
  [../]
  [./inertia_z]
    type = InertialForceFPI
    variable = disp_z
    displacement_old = disp_z_ref
    velocity = vel_z_ref
    acceleration = accel_z_ref
  [../]
[]

[BCs]
  [./top_y]
    type = DirichletBC
    variable = disp_y
    boundary = top
    value = 0.0
  [../]
  [./top_x]
    type = DirichletBC
    variable = disp_x
    boundary = top
    value = 0.0
  [../]
  [./top_z]
    type = DirichletBC
    variable = disp_z
    boundary = top
    value = 0.0
  [../]
  [./right_x]
    type = DirichletBC
    variable = disp_x
    boundary = right
    value = 0.0
  [../]
  [./right_z]
    type = DirichletBC
    variable = disp_z
    boundary = right
    value = 0.0
  [../]
  [./left_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0.0
  [../]
  [./left_z]
    type = DirichletBC
    variable = disp_z
    boundary = left
    value = 0.0
  [../]
  [./front_x]
    type = DirichletBC
    variable = disp_x
    boundary = front
    value = 0.0
  [../]
  [./front_z]
    type = DirichletBC
    variable = disp_z
    boundary = front
    value = 0.0
  [../]
  [./back_x]
    type = DirichletBC
    variable = disp_x
    boundary = back
    value = 0.0
  [../]
  [./back_z]
    type = DirichletBC
    variable = disp_z
    boundary = back
    value = 0.0
  [../]
  [./bottom_x]
    type = DirichletBC
    variable = disp_x
    boundary = bottom
    value = 0.0
  [../]
  [./bottom_z]
    type = DirichletBC
    variable = disp_z
    boundary = bottom
    value = 0.0
  [../]
  [./bottom_y]
    type = ScalarDirichletBC
    variable = disp_y
    boundary = bottom
    scalar_var = load
  [../]
[]

[Materials]
  [./Elasticity_tensor]
    type = ComputeElasticityTensor
    fill_method = symmetric_isotropic
    C_ijkl = '1 0'
  [../]
  [./strain]
    type = ComputeSmallStrain
    displacements = 'disp_x disp_y disp_z'
  [../]
  [./stress]
    type = ComputeLinearElasticStress
  [../]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '1'
  [../]
[]

[Executioner]
  type = Transient
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8
  dt = 0.005
[]

[Outputs]
  hide = 'load'
  print_linear_residuals = false
  exodus = true
[]

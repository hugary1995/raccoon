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

[Materials]
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

[Kernels]
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
    type = DirichletBC
    variable = disp_r
    boundary = 'bottom'
    value = 0
  [../]
  [./bottom_z_fix]
    type = DirichletBC
    variable = disp_z
    boundary = 'bottom'
    value = 0
  [../]
  [./top_z_disp]
    type = FunctionDirichletBC
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

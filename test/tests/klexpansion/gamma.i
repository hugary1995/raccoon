[Mesh]
  type = GeneratedMesh
  dim = 2
  xmin = 0
  xmax = 10
  ymin = 0
  ymax = 5
  nx = 100
  ny = 50
[]

[Modules]
  [./KLExpansion]
    file_name = 'gold/kl_info.txt'
    field_name = 'random_field'
    field_distribution = GAMMA
    perturbation = CUSTOM
    custom_Gaussian_weights = '-0.9 -0.7 -0.5 -0.3 -0.1 0.1 0.3 0.5 0.7 0.9'
    mean = 100
    CV = 0.30
  [../]
[]

[Variables]
  [./empty]
  [../]
[]

[Kernels]
  [./empty]
    type = NullKernel
    variable = empty
  [../]
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
[]

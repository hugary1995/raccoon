[Mesh]
  type = GeneratedMesh
  dim = 2
  xmin = -0.0042
  xmax = 0.0042
  ymin = -0.0042
  ymax = 0.0042
  nx = 200
  ny = 200
[]

[Modules]
  [./KLExpansion]
    file_name = 'gold/kl_info.txt'
    field_name = 'random_field'
    field_distribution = GAMMA
    perturbation = RANDOM
    mean = 120
    CV = 0.06
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

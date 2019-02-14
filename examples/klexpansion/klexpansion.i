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
    output_base = 'output/basis'
    file_name = 'gold/kl_info.txt'
    field_name = 'random_field'
    field_distribution = GAMMA
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
  [./exodus]
    type = Exodus
    file_base = output/klexpansion
  [../]
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Variables]
  [./d5]
  [../]
[]

[Kernels]
  [./empty]
    type = NullKernel
    variable = d5
  [../]
[]

[Materials]
  [./bulk]
    type = FractureMaterial
    d = d5
    Gc = 1
    L = 2
    local_dissipation_norm = 3
    outputs = exodus
  [../]
[]

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]

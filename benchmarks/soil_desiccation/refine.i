[Problem]
  type = FEProblem
  solve = false
[]

[Mesh]
  type = FileMesh
  file = 'gold/fields.e'
  uniform_refine = 1
[]

[AuxVariables]
  [Gc]
    initial_from_file_var = 'Gc'
  []
  [psic]
    initial_from_file_var = 'psic'
  []
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
  file_base = 'gold/fields_refined'
[]

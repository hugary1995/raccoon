[Problem]
  solve = false
[]

[Functions]
  [./Gc]
    type = PiecewiseMultilinear
    data_file = ../gold/Gc_sqexp_cartesian_10_10_rho_0_sample_5.txt
  [../]
  [./psic]
    type = PiecewiseMultilinear
    data_file = ../gold/psic_sqexp_cartesian_10_10_rho_0_sample_5.txt
  [../]
[]

[Mesh]
  type = FileMesh
  file = '../gold/geo.msh'
[]

[AuxVariables]
  [./Gc]
  [../]
  [./psic]
  [../]
[]

[AuxKernels]
  [./Gc]
    type = FunctionAux
    variable = Gc
    function = Gc
    execute_on = 'INITIAL'
  [../]
  [./psic]
    type = FunctionAux
    variable = psic
    function = psic
    execute_on = 'INITIAL'
  [../]
[]

[Executioner]
  type = Steady
[]

[Outputs]
  file_base = field
  exodus = true
[]

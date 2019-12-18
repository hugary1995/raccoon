[Functions]
  [./Gc_3]
    type = PiecewiseMultilinear
    data_file = '../gold/Gc_L_3_rho_0_sample_1.txt'
  [../]
  [./Gc_3_5]
    type = PiecewiseMultilinear
    data_file = '../gold/Gc_L_3.5_rho_0_sample_1.txt'
  [../]
  [./Gc_4]
    type = PiecewiseMultilinear
    data_file = '../gold/Gc_L_4_rho_0_sample_1.txt'
  [../]
  [./Gc_4_5]
    type = PiecewiseMultilinear
    data_file = '../gold/Gc_L_4.5_rho_0_sample_1.txt'
  [../]
  [./Gc_5]
    type = PiecewiseMultilinear
    data_file = '../gold/Gc_L_5_rho_0_sample_1.txt'
  [../]
  [./Gc_5_5]
    type = PiecewiseMultilinear
    data_file = '../gold/Gc_L_5.5_rho_0_sample_1.txt'
  [../]
  [./Gc_6]
    type = PiecewiseMultilinear
    data_file = '../gold/Gc_L_6_rho_0_sample_1.txt'
  [../]
  [./Gc_6_5]
    type = PiecewiseMultilinear
    data_file = '../gold/Gc_L_6.5_rho_0_sample_1.txt'
  [../]
  [./Gc_7]
    type = PiecewiseMultilinear
    data_file = '../gold/Gc_L_7_rho_0_sample_1.txt'
  [../]
  [./Gc_7_5]
    type = PiecewiseMultilinear
    data_file = '../gold/Gc_L_7.5_rho_0_sample_1.txt'
  [../]
  [./Gc_8]
    type = PiecewiseMultilinear
    data_file = '../gold/Gc_L_8_rho_0_sample_1.txt'
  [../]
  [./Gc_8_5]
    type = PiecewiseMultilinear
    data_file = '../gold/Gc_L_8.5_rho_0_sample_1.txt'
  [../]
[]

[Problem]
  solve = false
[]

[Mesh]
  type = FileMesh
  file = '../gold/2D.msh'
[]

[AuxVariables]
  [./Gc_3]
    [./InitialCondition]
      type = FunctionIC
      function = 'Gc_3'
    [../]
  [../]
  [./Gc_3_5]
    [./InitialCondition]
      type = FunctionIC
      function = 'Gc_3_5'
    [../]
  [../]
  [./Gc_4]
    [./InitialCondition]
      type = FunctionIC
      function = 'Gc_4'
    [../]
  [../]
  [./Gc_4_5]
    [./InitialCondition]
      type = FunctionIC
      function = 'Gc_4_5'
    [../]
  [../]
  [./Gc_5]
    [./InitialCondition]
      type = FunctionIC
      function = 'Gc_5'
    [../]
  [../]
  [./Gc_5_5]
    [./InitialCondition]
      type = FunctionIC
      function = 'Gc_5_5'
    [../]
  [../]
  [./Gc_6]
    [./InitialCondition]
      type = FunctionIC
      function = 'Gc_6'
    [../]
  [../]
  [./Gc_6_5]
    [./InitialCondition]
      type = FunctionIC
      function = 'Gc_6_5'
    [../]
  [../]
  [./Gc_7]
    [./InitialCondition]
      type = FunctionIC
      function = 'Gc_7'
    [../]
  [../]
  [./Gc_7_5]
    [./InitialCondition]
      type = FunctionIC
      function = 'Gc_7_5'
    [../]
  [../]
  [./Gc_8]
    [./InitialCondition]
      type = FunctionIC
      function = 'Gc_8'
    [../]
  [../]
  [./Gc_8_5]
    [./InitialCondition]
      type = FunctionIC
      function = 'Gc_8_5'
    [../]
  [../]
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
[]

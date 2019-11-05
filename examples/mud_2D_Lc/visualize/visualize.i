[Functions]
  [./Gc_sqexp_Lc_5]
    type = PiecewiseMultilinear
    data_file = '../Lc_5/gold/Gc_Lc_5_sample_1.txt'
  [../]
  [./Gc_sqexp_Lc_10]
    type = PiecewiseMultilinear
    data_file = '../Lc_10/gold/Gc_Lc_10_sample_1.txt'
  [../]
  [./Gc_sqexp_Lc_15]
    type = PiecewiseMultilinear
    data_file = '../Lc_15/gold/Gc_Lc_15_sample_1.txt'
  [../]
  [./Gc_sqexp_Lc_20]
    type = PiecewiseMultilinear
    data_file = '../Lc_20/gold/Gc_Lc_20_sample_1.txt'
  [../]

  [./psic_sqexp_Lc_5]
    type = PiecewiseMultilinear
    data_file = '../Lc_5/gold/psic_Lc_5_sample_1.txt'
  [../]
  [./psic_sqexp_Lc_10]
    type = PiecewiseMultilinear
    data_file = '../Lc_10/gold/psic_Lc_10_sample_1.txt'
  [../]
  [./psic_sqexp_Lc_15]
    type = PiecewiseMultilinear
    data_file = '../Lc_15/gold/psic_Lc_15_sample_1.txt'
  [../]
  [./psic_sqexp_Lc_20]
    type = PiecewiseMultilinear
    data_file = '../Lc_20/gold/psic_Lc_20_sample_1.txt'
  [../]

  [./Gc_exp_Lc_5]
    type = PiecewiseMultilinear
    data_file = 'Gc_exp_Lc_5.txt'
  [../]
  [./Gc_exp_Lc_10]
    type = PiecewiseMultilinear
    data_file = 'Gc_exp_Lc_10.txt'
  [../]
  [./Gc_exp_Lc_15]
    type = PiecewiseMultilinear
    data_file = 'Gc_exp_Lc_15.txt'
  [../]
  [./Gc_exp_Lc_20]
    type = PiecewiseMultilinear
    data_file = 'Gc_exp_Lc_20.txt'
  [../]

  [./Gc_brick]
    type = PiecewiseMultilinear
    data_file = 'Gc_brick.txt'
  [../]
  [./Gc_brick_rotated]
    type = PiecewiseMultilinear
    data_file = 'Gc_brick_rotated.txt'
  [../]
  [./Gc_radial]
    type = PiecewiseMultilinear
    data_file = 'Gc_radial.txt'
  [../]
  [./Gc_ring]
    type = PiecewiseMultilinear
    data_file = 'Gc_ring.txt'
  [../]

  [./Gc_rho_0]
    type = PiecewiseMultilinear
    data_file = 'Gc_rho_0.txt'
  [../]

  [./psic_rho_0]
    type = PiecewiseMultilinear
    data_file = 'psic_rho_0.txt'
  [../]
  [./psic_rho_0.25]
    type = PiecewiseMultilinear
    data_file = 'psic_rho_0.25.txt'
  [../]
  [./psic_rho_0.5]
    type = PiecewiseMultilinear
    data_file = 'psic_rho_0.5.txt'
  [../]
  [./psic_rho_0.75]
    type = PiecewiseMultilinear
    data_file = 'psic_rho_0.75.txt'
  [../]
  [./psic_rho_1]
    type = PiecewiseMultilinear
    data_file = 'psic_rho_1.txt'
  [../]

  [./Gc_sqexp]
    type = PiecewiseMultilinear
    data_file = '/home/gary/projects/raccoon/examples/mud_2D_rho/run_1/gold/Gc_sample_1_rho_0.txt'
  [../]
  [./psic_sqexp]
    type = PiecewiseMultilinear
    data_file = '/home/gary/projects/raccoon/examples/mud_2D_rho/run_1/gold/psic_sample_1_rho_0.txt'
  [../]

  [./Gc_exp]
    type = PiecewiseMultilinear
    data_file = '/home/gary/projects/raccoon/examples/mud_2D_rough/run_1/gold/Gc_sample_1_rho_0.txt'
  [../]
  [./psic_exp]
    type = PiecewiseMultilinear
    data_file = '/home/gary/projects/raccoon/examples/mud_2D_rough/run_1/gold/psic_sample_1_rho_0.txt'
  [../]
[]

[Mesh]
  type = FileMesh
  file = '../gold/geo.msh'
[]

[Problem]
  solve = false
[]

[AuxVariables]
  [./Gc_sqexp_Lc_5]
  [../]
  [./Gc_sqexp_Lc_10]
  [../]
  [./Gc_sqexp_Lc_15]
  [../]
  [./Gc_sqexp_Lc_20]
  [../]

  [./psic_sqexp_Lc_5]
  [../]
  [./psic_sqexp_Lc_10]
  [../]
  [./psic_sqexp_Lc_15]
  [../]
  [./psic_sqexp_Lc_20]
  [../]

  [./Gc_exp_Lc_5]
  [../]
  [./Gc_exp_Lc_10]
  [../]
  [./Gc_exp_Lc_15]
  [../]
  [./Gc_exp_Lc_20]
  [../]

  [./Gc_brick]
  [../]
  [./Gc_brick_rotated]
  [../]
  [./Gc_radial]
  [../]
  [./Gc_ring]
  [../]

  [./Gc_rho_0]
  [../]

  [./psic_rho_0]
  [../]
  [./psic_rho_0.25]
  [../]
  [./psic_rho_0.5]
  [../]
  [./psic_rho_0.75]
  [../]
  [./psic_rho_1]
  [../]

  [./Gc_sqexp]
  [../]
  [./psic_sqexp]
  [../]

  [./Gc_exp]
  [../]
  [./psic_exp]
  [../]
[]

[AuxKernels]
  [./Gc_sqexp_Lc_5]
    type = FunctionAux
    variable = Gc_sqexp_Lc_5
    function = Gc_sqexp_Lc_5
    execute_on = 'INITIAL'
  [../]
  [./Gc_sqexp_Lc_10]
    type = FunctionAux
    variable = Gc_sqexp_Lc_10
    function = Gc_sqexp_Lc_10
    execute_on = 'INITIAL'
  [../]
  [./Gc_sqexp_Lc_15]
    type = FunctionAux
    variable = Gc_sqexp_Lc_15
    function = Gc_sqexp_Lc_15
    execute_on = 'INITIAL'
  [../]
  [./Gc_sqexp_Lc_20]
    type = FunctionAux
    variable = Gc_sqexp_Lc_20
    function = Gc_sqexp_Lc_20
    execute_on = 'INITIAL'
  [../]

  [./psic_sqexp_Lc_5]
    type = FunctionAux
    variable = psic_sqexp_Lc_5
    function = psic_sqexp_Lc_5
    execute_on = 'INITIAL'
  [../]
  [./psic_sqexp_Lc_10]
    type = FunctionAux
    variable = psic_sqexp_Lc_10
    function = psic_sqexp_Lc_10
    execute_on = 'INITIAL'
  [../]
  [./psic_sqexp_Lc_15]
    type = FunctionAux
    variable = psic_sqexp_Lc_15
    function = psic_sqexp_Lc_15
    execute_on = 'INITIAL'
  [../]
  [./psic_sqexp_Lc_20]
    type = FunctionAux
    variable = psic_sqexp_Lc_20
    function = psic_sqexp_Lc_20
    execute_on = 'INITIAL'
  [../]

  [./Gc_exp_Lc_5]
    type = FunctionAux
    variable = Gc_exp_Lc_5
    function = Gc_exp_Lc_5
    execute_on = 'INITIAL'
  [../]
  [./Gc_exp_Lc_10]
    type = FunctionAux
    variable = Gc_exp_Lc_10
    function = Gc_exp_Lc_10
    execute_on = 'INITIAL'
  [../]
  [./Gc_exp_Lc_15]
    type = FunctionAux
    variable = Gc_exp_Lc_15
    function = Gc_exp_Lc_15
    execute_on = 'INITIAL'
  [../]
  [./Gc_exp_Lc_20]
    type = FunctionAux
    variable = Gc_exp_Lc_20
    function = Gc_exp_Lc_20
    execute_on = 'INITIAL'
  [../]

  [./Gc_brick]
    type = FunctionAux
    variable = Gc_brick
    function = Gc_brick
    execute_on = 'INITIAL'
  [../]
  [./Gc_brick_rotated]
    type = FunctionAux
    variable = Gc_brick_rotated
    function = Gc_brick_rotated
    execute_on = 'INITIAL'
  [../]
  [./Gc_radial]
    type = FunctionAux
    variable = Gc_radial
    function = Gc_radial
    execute_on = 'INITIAL'
  [../]
  [./Gc_ring]
    type = FunctionAux
    variable = Gc_ring
    function = Gc_ring
    execute_on = 'INITIAL'
  [../]

  [./Gc_rho_0]
    type = FunctionAux
    variable = Gc_rho_0
    function = Gc_rho_0
    execute_on = 'INITIAL'
  [../]

  [./psic_rho_0]
    type = FunctionAux
    variable = psic_rho_0
    function = psic_rho_0
    execute_on = 'INITIAL'
  [../]
  [./psic_rho_0.25]
    type = FunctionAux
    variable = psic_rho_0.25
    function = psic_rho_0.25
    execute_on = 'INITIAL'
  [../]
  [./psic_rho_0.5]
    type = FunctionAux
    variable = psic_rho_0.5
    function = psic_rho_0.5
    execute_on = 'INITIAL'
  [../]
  [./psic_rho_0.75]
    type = FunctionAux
    variable = psic_rho_0.75
    function = psic_rho_0.75
    execute_on = 'INITIAL'
  [../]
  [./psic_rho_1]
    type = FunctionAux
    variable = psic_rho_1
    function = psic_rho_1
    execute_on = 'INITIAL'
  [../]

  [./Gc_sqexp]
    type = FunctionAux
    variable = Gc_sqexp
    function = Gc_sqexp
    execute_on = 'INITIAL'
  [../]
  [./psic_sqexp]
    type = FunctionAux
    variable = psic_sqexp
    function = psic_sqexp
    execute_on = 'INITIAL'
  [../]

  [./Gc_exp]
    type = FunctionAux
    variable = Gc_exp
    function = Gc_exp
    execute_on = 'INITIAL'
  [../]
  [./psic_exp]
    type = FunctionAux
    variable = psic_exp
    function = psic_exp
    execute_on = 'INITIAL'
  [../]
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
[]

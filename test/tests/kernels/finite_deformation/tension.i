[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./stress_xx]
    family = MONOMIAL
  [../]
  [./stress_yy]
    family = MONOMIAL
  [../]
  [./stress_xy]
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./stress_xx]
    type = RankTwoAux
    variable = stress_xx
    rank_two_tensor = 'stress'
    index_i = 0
    index_j = 0
  [../]
  [./stress_yy]
    type = RankTwoAux
    variable = stress_yy
    rank_two_tensor = 'stress'
    index_i = 1
    index_j = 1
  [../]
  [./stress_xy]
    type = RankTwoAux
    variable = stress_xy
    rank_two_tensor = 'stress'
    index_i = 0
    index_j = 1
  [../]
[]

[Kernels]
  [./solid_x]
    type = PiolaKirchhoffStressDivergence
    variable = disp_x
    component = 0
  [../]
  [./solid_y]
    type = PiolaKirchhoffStressDivergence
    variable = disp_y
    component = 1
  [../]
[]

[BCs]
  [./xdisp_top]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 'top'
    function = '0'
  [../]
  [./xdisp_bottom]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 'bottom'
    function = '0'
  [../]
  [./ydisp_top]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'top'
    function = 't'
  [../]
  [./ydisp_bottom]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'bottom'
    function = '0'
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeElasticityTensor
    C_ijkl = '120.0 80.0'
    fill_method = symmetric_isotropic
  [../]
  [./greem_strain]
    type = GreenStrain
  [../]
  [./second_piola_kirchhoff_stress]
    type = GreenStrainLinearElasticStress
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient

  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  dt = 0.1
  num_steps = 10
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

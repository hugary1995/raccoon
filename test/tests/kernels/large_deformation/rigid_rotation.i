[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[MeshModifiers]
  [./left_right]
    type = AddExtraNodeset
    coord = '0 0'
    new_boundary = 100
  [../]
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
    rank_two_tensor = 'cauchy_stress'
    index_i = 0
    index_j = 0
  [../]
  [./stress_yy]
    type = RankTwoAux
    variable = stress_yy
    rank_two_tensor = 'cauchy_stress'
    index_i = 1
    index_j = 1
  [../]
  [./stress_xy]
    type = RankTwoAux
    variable = stress_xy
    rank_two_tensor = 'cauchy_stress'
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
  [./ydisp]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'bottom'
    function = 'if( t<=10, 0, x*sin((t-10)*pi/20) )'
  [../]
  [./xdisp]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 'bottom'
    function = 'if( t<=10, 0, x*(cos((t-10)*pi/20)-1) )'
  [../]
  [./Pressure]
    [./top]
      boundary = 'top'
      function = 'if(t<=10,-t,-10)'
      displacements = 'disp_x disp_y'
    [../]
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 100
    poissons_ratio = 0
  [../]
  [./greem_strain]
    type = GreenStrain
  [../]
  [./second_piola_kirchhoff_stress]
    type = GreenStrainLinearElasticStress
    compute_cauchy_stress = true
  [../]
[]

[Postprocessors]
  [./stress_xx]
    type = ElementalVariableValue
    variable = stress_xx
    elementid = 1
  [../]
  [./stress_yy]
    type = ElementalVariableValue
    variable = stress_yy
    elementid = 1
  [../]
  [./stress_xy]
    type = ElementalVariableValue
    variable = stress_xy
    elementid = 1
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

  nl_abs_tol = 1e-50
  nl_rel_tol = 1e-12

  dt = 1
  num_steps = 20
[]

[Outputs]
  exodus = true
  csv = true
  print_linear_residuals = false
[]

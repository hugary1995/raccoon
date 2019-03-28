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
  [./ydisp_bottom]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'bottom'
    function = 'if( t<=10, 0, x*sin((t-10)*pi/20) )'
  [../]
  [./xdisp_bottom]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 'bottom'
    function = 'if( t<=10, 0, x*(cos((t-10)*pi/20)-1) )'
  [../]
  [./xpressure_top]
    type = PressureBC
    variable = disp_x
    component = 0
    function = 'if( t<=10, -5*t, -50 )'
    boundary = 'top'
  [../]
  [./ypressure_top]
    type = PressureBC
    variable = disp_y
    component = 1
    function = 'if( t<=10, -5*t, -50 )'
    boundary = 'top'
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
  nl_rel_tol = 1e-6

  dt = 1
  end_time = 20

  abort_on_solve_fail = true
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

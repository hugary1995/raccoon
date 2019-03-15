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
  [./ydisp]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'top bottom left right'
    function = 'y*(cos(t)-1)+x*sin(t)'
  [../]
  [./xdisp]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 'top bottom left right'
    function = 'x*(cos(t)-1)-y*sin(t)'
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeElasticityTensor
    C_ijkl = '120.0 80.0'
    fill_method = symmetric_isotropic
  [../]
  [./greem_strain]
    type = ComputeSmallStrain
    outputs = exodus
  [../]
  [./second_piola_kirchhoff_stress]
    type = ComputeLinearElasticStress
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
  solve_type = PJFNK
  # solve_type = 'NEWTON'
  # petsc_options_iname = '-ksp_type -pc_type'
  # petsc_options_value = 'preonly   lu'
  dt = 0.1
  num_steps = 10
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

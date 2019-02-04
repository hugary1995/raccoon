[GlobalParams]
  outputs = exodus
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Variables]
  [./d]
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = d
  [../]
  [./TensorMechanics]
    displacements = 'disp_x disp_y'
  [../]
  [./offdiag_x]
    type = PhaseFieldFractureMechanicsOffDiag
    component = 0
    variable = disp_x
    c = d
  [../]
  [./offdiag_y]
    type = PhaseFieldFractureMechanicsOffDiag
    component = 1
    variable = disp_y
    c = d
  [../]
[]

[BCs]
  [./left]
    type = FunctionDirichletBC
    variable = d
    boundary = left
    function = 0
  [../]
  [./right]
    type = FunctionDirichletBC
    variable = d
    boundary = right
    function = t
  [../]
  [./top_disp_x]
    type = DirichletBC
    variable = disp_x
    boundary = top
    value = 0
  [../]
  [./top_disp_y]
    type = DirichletBC
    variable = disp_y
    boundary = top
    value = 0.05
  [../]
  [./bottom_disp_x]
    type = DirichletBC
    variable = disp_x
    boundary = bottom
    value = 0
  [../]
  [./bottom_disp_y]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
  [../]
[]

[Materials]
  [./degradation]
    type = Degradation
    f_name = 'g_d'
    args = 'd'
    constant_names       = 'eta'
    constant_expressions = '0'
    function = '(1-d)^2*(1-eta)+eta'
  [../]
  [./lump]
    type = LumpedDegradation
    damage_fields = 'd'
  [../]
  [./Cijkl]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1
    poissons_ratio = 0.2
  [../]
  [./strain]
    type = ComputeSmallStrain
    displacements = 'disp_x disp_y'
  [../]
  [./stress]
    type = ComputeDegradedStress
    damage_fields = 'd'
    decomposition = strain_spectral
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
  petsc_options_iname = '-ksp-type -pc_type'
  petsc_options_value = 'preonly   lu'
  dt = 0.1
  end_time = 1
[]

[Outputs]
  exodus = true
[]

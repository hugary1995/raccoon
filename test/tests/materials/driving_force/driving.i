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
    function = t
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
  [./bulk]
    type = BrittleFractureMaterial
    d = d
    Gc = 1
    L = 0.1
    local_dissipation_norm = 0.5
  [../]
  [./degradation]
    type = DerivativeParsedMaterial
    f_name = 'g_d'
    args = 'd'
    function = '(1-d)^2'
    derivative_order = 2
    tol_names = 'd'
    tol_values = 1e-6
  [../]
  [./local]
    type = DerivativeParsedMaterial
    f_name = 'w_d'
    args = 'd'
    function = 'd^2'
    derivative_order = 2
  [../]
  [./lump]
    type = LumpedDegradation
    damage_fields = 'd'
  [../]
  [./Cijkl]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1
    poissons_ratio = 0.2
    outputs = none
  [../]
  [./strain]
    type = ComputeSmallStrain
    displacements = 'disp_x disp_y'
    outputs = none
  [../]
  [./stress]
    type = LinearElasticDegradedStress
    damage_fields = 'd'
    decomposition = NO_DECOMP
  [../]
  [./driving_energy]
    type = FractureDrivingForce
    damage_fields = 'd'
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

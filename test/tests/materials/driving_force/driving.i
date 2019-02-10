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
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./d]
  [../]
[../]

[Kernels]
  [./TensorMechanics]
    displacements = 'disp_x disp_y'
  [../]
[]

[AuxKernels]
  [./d]
    type = FunctionAux
    variable = d
    function = t
  [../]
[]

[BCs]
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
    type = FractureMaterial
    d = d
    Gc = 1
    L = 0.1
    local_dissipation_norm = 0.5
  [../]
  [./degradation]
    type = DerivativeParsedMaterial
    f_name = 'g_d'
    args = 'd'
    constant_names = 'eta'
    constant_expressions = '1e-6'
    function = '(1-d)^2*(1-eta)+eta'
    derivative_order = 2
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
    poissons_ratio = 0.3
    outputs = none
  [../]
  [./strain]
    type = ComputeSmallStrain
    displacements = 'disp_x disp_y'
    outputs = none
  [../]
  [./stress]
    type = SmallStrainElasticDegradedStress
    damage_fields = 'd'
  [../]
  [./driving_energy]
    type = DerivativeSumMaterial
    args = 'd'
    derivative_order = 2
    f_name = 'D_d'
    sum_materials = 'E_el_d w_d'
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
  nl_abs_tol = 1e-6
  dt = 0.1
  end_time = 1
[]

[Outputs]
  exodus = true
[]

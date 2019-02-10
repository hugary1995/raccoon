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
[]

[Kernels]
  [./TensorMechanics]
    displacements = 'disp_x disp_y'
  [../]
[]

[AuxKernels]
  [./d]
    type = FunctionAux
    variable = d
    function = t*x
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
  [./mobility]
    type = GenericConstantMaterial
    prop_names = 'M_d'
    prop_values = '1'
  [../]
  [./degradation]
    type = DerivativeParsedMaterial
    f_name = 'g_d'
    args = 'd'
    constant_names       = 'eta'
    constant_expressions = '0'
    function = '(1-d)^2*(1-eta)+eta'
    derivative_order = 2
  [../]
  [./lump]
    type = LumpedDegradation
    damage_fields = 'd'
    degradation_name = g
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
    type = SmallStrainElasticDegradedStress
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

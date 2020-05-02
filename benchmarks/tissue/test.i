[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Mesh]
  [./fmg]
    type = FileMeshGenerator
    file = 'gold/geo3.msh'
  [../]
  second_order = true
[]

[Variables]
  [./disp_x]
    order = SECOND
  [../]
  [./disp_y]
    order = SECOND
  [../]
  [./disp_z]
    order = SECOND
  [../]
[]

[AuxVariables]
  [./stress]
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./stress]
    type = ADRankTwoScalarAux
    variable = 'stress'
    rank_two_tensor = 'cauchy_stress'
    scalar_type = 'MaxPrincipal'
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    component = 0
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
  [../]
  [./solid_z]
    type = ADStressDivergenceTensors
    variable = 'disp_z'
    component = 2
  [../]
[]

[Materials]
  [./RCG]
    type = RCGStrain
  [../]

  [./bulk]
    type = GenericConstantMaterial
    prop_names = 'eta1 eta2 eta3 k1 k2'
    prop_values = '141 160 3100 1e6 0.04'
  [../]
  [./tissue_orientation_1]
    type = GenericConstantRankTwoTensor
    tensor_name = 'M1'
    tensor_values = '0.5 0.5 0 0 0 0.5'
  [../]
  [./tissue_orientation_2]
    type = GenericConstantRankTwoTensor
    tensor_name = 'M2'
    tensor_values = '0.5 0.5 0 0 0 -0.5'
  [../]
  [./stress_MR]
    type = MooneyRivlinElasticPK1Stress
    base_name = 'MR'
  [../]
  [./stress_penalty]
    type = IncompressibilityPenaltyElasticPK1Stress
    base_name = 'penalty'
    epsilon1 = 0.005
    epsilon2 = 10
  [../]
  [./stress_tissue_1]
    type = TissueElasticPK1Stress
    base_name = 'tissue1'
    M = 'M1'
  [../]
  [./stress_tissue_2]
    type = TissueElasticPK1Stress
    base_name = 'tissue2'
    M = 'M2'
  [../]

  [./PK1_stress]
    type = SumStress
    base_names = 'MR'
  [../]

  [./cauchy_stress]
    type = SumStress
    base_names = 'MR'
    stress_name = 'cauchy_stress'
  [../]
[]

[BCs]
  [./Pressure]
    [./inner]
      boundary = 'inner'
      function = 't'
    [../]
  [../]
  # [./zfix]
  #   type = DirichletBC
  #   variable = 'disp_z'
  #   boundary = 'top bottom'
  #   value = 0
  # [../]
  # [./yfix]
  #   type = DirichletBC
  #   variable = 'disp_y'
  #   boundary = 'pin'
  #   value = 0
  # [../]
  # [./xfix]
  #   type = DirichletBC
  #   variable = 'disp_x'
  #   boundary = 'pin'
  #   value = 0
  # [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'

  line_search = none

  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  petsc_options_value = 'asm      lu          1000        200                0                    '

  # petsc_options_iname = '-pc_type'
  # petsc_options_value = 'lu'

  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08
  nl_max_its = 100
  dt = 1e-2
  end_time = 0.25

  # automatic_scaling = true
  # compute_scaling_once = false
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

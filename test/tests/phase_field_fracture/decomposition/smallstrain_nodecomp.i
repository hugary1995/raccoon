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
  [./d]
  [../]
[../]

[AuxKernels]
  [./d]
    type = FunctionAux
    variable = d
    function = t
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
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
    type = FunctionDirichletBC
    variable = disp_y
    boundary = top
    function = '0.05'
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
  [./b]
    type = FractureEnergyBarrier
    initial_degradation_slope = -2*(1-1e-06)
    initial_local_dissipation_slope = 0
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = 1
    L = 0.1
    local_dissipation_norm = 2
  [../]
  [./degradation]
    type = QuadraticDegradation
    d = d
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 100
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = ADComputeSmallStrain
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_NoSplit
    history = false
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  dt = 0.1
  end_time = 1

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-08
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

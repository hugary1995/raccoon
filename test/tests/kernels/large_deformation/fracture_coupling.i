[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  xmax = 0.2
  ymax = 0.2
  nx = 10
  ny = 10
[]

[Modules]
  [./PhaseFieldFracture]
    [./CohesiveFracture]
      [./d]
        viscosity = 0
        Gc = 2.7
        L = 0.05
        fracture_energy_barrier = 10
        residual_degradation = 0
      [../]
    [../]
    [./ElasticCoupling]
      [./all]
        damage_fields = 'd'
        strain = FINITE
        decomposition = STRAIN_SPECTRAL
        irreversibility = HISTORY
      [../]
    [../]
  [../]
[]

[BCs]
  [./yfix]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  [../]
  [./xfix]
    type = DirichletBC
    variable = disp_x
    boundary = 'bottom'
    value = 0
  [../]
  [./xforce]
    type = FollowerForceBC
    variable = disp_x
    component = 0
    boundary = 'top'
    function_x = '0.2*t'
  [../]
  [./yforce]
    type = FollowerForceBC
    variable = disp_y
    component = 1
    boundary = 'top'
    function_x = '0.2*t'
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.1e5
    poissons_ratio = 0.3
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
  nl_rel_tol = 1e-06

  dt = 1
  end_time = 2000
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
  interval = 1
[]

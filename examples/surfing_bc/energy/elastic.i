[Mesh]
  type = FileMesh
  file = 'gold/geo.msh'
[]

[Variables]
  [./disp_x]
    order = SECOND
  [../]
  [./disp_y]
    order = SECOND
  [../]
[]

[AuxVariables]
  [./fx]
    order = SECOND
  [../]
  [./fy]
    order = SECOND
  [../]
  [./a]
    order = FIRST
    family = SCALAR
  [../]
[]

[AuxScalarKernels]
  [./a]
    type = FunctionScalarAux
    variable = a
    function = 't'
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    displacements = 'disp_x disp_y'
    save_in = 'fx'
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    displacements = 'disp_x disp_y'
    save_in = 'fy'
  [../]
[]

[BCs]
  [./x]
    type = ModeISurfingDirichletBC
    variable = disp_x
    boundary = 'top left right'
    component = 0
    E = 2.1e5
    nu = 0.3
    Gc = 2.7
    crack_tip_x = a
    crack_tip_y = 0
  [../]
  [./y]
    type = ModeISurfingDirichletBC
    variable = disp_y
    boundary = 'top left right'
    component = 1
    E = 2.1e5
    nu = 0.3
    Gc = 2.7
    crack_tip_x = a
    crack_tip_y = 0
  [../]
  [./symmetry]
    type = DirichletBC
    variable = disp_y
    value = 0
    boundary = 'bottom'
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.1e5
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = RCGStrain
    displacements = 'disp_x disp_y'
  [../]
  [./elastic]
    type = CNHPK1Stress
  [../]
[]

[Postprocessors]
  [./external_energy]
    type = ExternalEnergy
    boundary = 'top bottom left right'
    displacements = 'disp_x disp_y'
    forces = 'fx fy'
  [../]
  [./elastic_energy]
    type = ElementIntegralMaterialProperty
    mat_prop = 'elastic_energy'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  # petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  # petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  petsc_options_value = 'asm      ilu          1000        200                0                    '
  dt = 2.5e-3
  start_time = 0.1
  end_time = 0.6

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06
  nl_max_its = 200

  automatic_scaling = true
[]

[Outputs]
  file_base = 'elastic'
  print_linear_residuals = false
  hide = 'a'
  csv = true
  exodus = true
[]

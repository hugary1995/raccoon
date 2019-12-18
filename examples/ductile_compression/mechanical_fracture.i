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
  [./d]
    order = SECOND
  [../]
[]

[AuxVariables]
  [./bounds_dummy]
    order = SECOND
  [../]
  [./alpha]
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./alpha]
    type = MaterialRealAux
    property = 'alpha'
    variable = 'alpha'
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Bounds]
  [./irreversibility]
    type = Irreversibility
    variable = bounds_dummy
    bounded_variable = d
    upper = 1
    lower = d
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    displacements = 'disp_x disp_y'
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    displacements = 'disp_x disp_y'
  [../]
  [./react]
    type = PhaseFieldFractureEvolutionReaction
    variable = d
    driving_energy_name = E_el
  [../]
  [./diff]
    type = PhaseFieldFractureEvolutionDiffusion
    variable = d
    driving_energy_name = E_el
  [../]
[]

[BCs]
  [./fix_bottom_x]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'bottom'
    value = 0
  [../]
  [./fix_bottom_y]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'bottom'
    value = 0
  [../]
  [./disp_top_y]
    type = FunctionDirichletBC
    variable = 'disp_y'
    boundary = 'top'
    function = '-t'
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1.1e5
    poissons_ratio = 0.343
  [../]
  [./strain]
    type = GreenStrain
    displacements = 'disp_x disp_y'
  [../]
  [./elastic]
    type = CNHDegradedPK1ElastoPlasticStress_VolDev
    d = d
    history = false
    W0 = 200
    linear_hardening_coefficient = 700
    yield_stress = 300
  [../]
  [./fracture_properties]
    type = GenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length b'
    prop_values = '45 1 2.2'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./phase_field_properties]
    type = FractureMaterial
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-06
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  # petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  # petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type'
  petsc_options_value = 'asm      ilu          200         200                 0                     vinewtonrsls'
  dt = 0.05
  start_time = 0
  end_time = 10

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-08

  automatic_scaling = true
  compute_scaling_once = false
[]

[Outputs]
  print_linear_residuals = false
  exodus = true
[]

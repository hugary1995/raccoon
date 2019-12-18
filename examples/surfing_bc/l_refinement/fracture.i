[Mesh]
  type = FileMesh
  file = 'gold/geo.msh'
[]

[Variables]
  [./d]
    order = SECOND
  [../]
[]

[AuxVariables]
  [./d_ref]
    order = SECOND
  [../]
  [./bounds_dummy]
    order = SECOND
  [../]
  [./D]
    family = MONOMIAL
  [../]
[]

[Bounds]
  [./irreversibility]
    type = Irreversibility
    variable = bounds_dummy
    bounded_variable = d
    upper = 1
    lower = 'd_ref'
  [../]
[]

[Kernels]
  [./react]
    type = PhaseFieldFractureEvolutionReaction
    variable = 'd'
    driving_energy_var = 'D'
  [../]
  [./diff]
    type = PhaseFieldFractureEvolutionDiffusion
    variable = 'd'
    driving_energy_var = 'D'
  [../]
[]

[Materials]
  [./fracture_properties]
    type = GenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length b'
    prop_values = '2.7 ${L} 14.88'
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
    residual_degradation = 1e-09
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'
  dt = 1e-4

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06
[]

[Outputs]
  print_linear_residuals = false
[]

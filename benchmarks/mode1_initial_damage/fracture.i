[Mesh]
  type = FileMesh
  file = 'gold/geo.msh'
[]

[Variables]
  [./d]
  [../]
[]

[AuxVariables]
  [./load]
    family = SCALAR
  [../]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./d_ref]
  [../]
  [./bounds_dummy]
  [../]
[]

[Bounds]
  [./irreversibility]
    type = Irreversibility
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    upper = 1
    lower = 'd_ref'
  [../]
[]

[Kernels]
  [./react]
    type = PhaseFieldFractureEvolutionReaction
    variable = 'd'
    driving_energy_var = 'E_el'
  [../]
  [./diff]
    type = PhaseFieldFractureEvolutionDiffusion
    variable = 'd'
  [../]
[]

[Materials]
  [./fracture_energy_barrier]
    type = GenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length critical_fracture_energy'
    prop_values = '${Gc} ${l} ${psic}'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = ${k}
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu vinewtonrsls'
  dt = 1e-6

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06
[]

[Outputs]
  print_linear_residuals = false
  [./console]
    type = Console
    hide = 'load'
    outlier_variable_norms = false
  [../]
[]

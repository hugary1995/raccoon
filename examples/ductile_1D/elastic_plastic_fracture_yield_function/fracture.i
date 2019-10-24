[Mesh]
  type = GeneratedMesh
  dim = 3
  elem_type = HEX8
[]

[Variables]
  [./d]
  [../]
[]

[AuxVariables]
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
    variable = bounds_dummy
    bounded_variable = d
    upper = 1
    lower = d_ref
  [../]
[]

[Kernels]
  [./damage]
    type = PhaseFieldFractureEvolution
    variable = d
    driving_energy_var = E_el
  [../]
[]

[Materials]
  [./fracture_energy_barrier]
    type = FractureEnergyBarrier
    initial_local_dissipation_slope = 0
    initial_degradation_slope = -2
  [../]
  [./local_dissipation]
    type = QuadraticLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = 68.9
    L = 2
    local_dissipation_norm = 2
  [../]
  [./degradation]
    type = QuadraticDegradation
    d = d
    residual_degradation = 1e-12
  [../]
[]

[Executioner]
  type = TransientSupNorm
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu vinewtonrsls'
  dt = 1e-5
  dtmin = 1e-5
  end_time = 0.4

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-10
[]

[Outputs]
  print_linear_residuals = false
[]

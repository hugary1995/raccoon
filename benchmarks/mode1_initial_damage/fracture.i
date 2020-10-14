[Mesh]
  [square]
    type = GeneratedMeshGenerator
    dim = 2
    xmin = -0.5
    xmax = 0.5
    ymin = -0.5
    ymax = 0.5
    nx = 26
    ny = 26
  []
[]

[Adaptivity]
  marker = 'box'
  max_h_level = 3
  initial_steps = 3
  [Markers]
    [box]
      type = BoxMarker
      bottom_left = '-0.5 -0.04 0'
      top_right = '0.5 0.04 0'
      inside = refine
      outside = do_nothing
    []
  []
[]

[Variables]
  [d]
    [InitialCondition]
      type = CohesiveDamageIC
      d0 = 1.0
      l = ${l}
      x1 = -0.5
      y1 = 0
      z1 = 0
      x2 = 0
      y2 = 0
      z2 = 0
    []
  []
[]

[AuxVariables]
  [E_el_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [bounds_dummy]
  []
[]

[Bounds]
  [irreversibility]
    type = VariableOldValueBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = lower
  []
  [upper]
    type = ConstantBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = upper
    bound_value = 1
  []
[]

[Kernels]
  [diff]
    type = ADPFFDiffusion
    variable = 'd'
  []
  [barrier]
    type = ADPFFBarrier
    variable = 'd'
  []
  [react]
    type = ADPFFReaction
    variable = 'd'
    driving_energy_var = 'E_el_active'
  []
[]

[Materials]
  [fracture_bulk]
    type = ADGenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length critical_fracture_energy'
    prop_values = '${Gc} ${l} ${psic}'
  []
  [local_dissipation]
    type = LinearLocalDissipation
    d = d
  []
  [fracture_properties]
    type = ADFractureMaterial
    local_dissipation_norm = 8/3
  []
  [degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = ${k}
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu       vinewtonrsls'

  dt = 1e-4
  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08
[]

[Outputs]
  print_linear_residuals = false
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
[]

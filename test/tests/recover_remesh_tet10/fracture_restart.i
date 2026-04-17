# Phase-field fracture sub-app used by restart.i (TET10 variant).
# Mirrors fracture_ref.i but: (1) reads d from the reference Exodus via
# SolutionUserObjectQP, (2) uses the *Recover variants of ComputeDeformationGradient
# and ADPFFDiffusion so the sub-app exercises the same recovery code paths as
# the full simulation would.

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  element = TET10_4th
[]

[Mesh]
  # Read the reference run's Exodus output -- same mesh the parent app (restart.i)
  # uses -- so node/element IDs line up and recovery is bit-exact.
  [fmg]
    type = FileMeshGenerator
    file = ${recover_file}
  []
[]

[UserObjects]
  [epsol]
    type = SolutionUserObjectQP
    mesh = ${recover_file}
    system_variables = 'd'
    materials = ''
    tensor_materials = 'rotation_tensor stretch_tensor'
    nodal_variable_order = SECOND
    timestep = LATEST
    use_displaced_mesh = false
    execute_on = 'INITIAL'
    allow_duplicate_execution_on_initial = true
  []
[]

[Variables]
  [d]
    order = SECOND
    [InitialCondition]
      type = SolutionIC
      from_variable = d
      variable = d
      solution_uo = epsol
    []
  []
[]

[AuxVariables]
  [psie_corr_active]
    order = FIRST
    family = MONOMIAL
  []
  [psip_active]
    order = FIRST
    family = MONOMIAL
  []
  [coal]
    order = CONSTANT
    family = MONOMIAL
  []
  # ComputeDeformationGradient(recover = true) needs displacement vars coupled
  # in so the Fbar material can be rebuilt; fracture sub-app does not solve for
  # them so they are AuxVariables here (not Variables).
  [disp_x]
    order = SECOND
  []
  [disp_y]
    order = SECOND
  []
  [disp_z]
    order = SECOND
  []
[]

# Irreversibility is enforced in the parent (mechanics) app via d_corr =
# min(1, max(d_old, max(0, d))), so the sub-app solves an unconstrained d.
# No [Bounds] block and no vinewtonrsls solver.

[Kernels]
  [source]
    type = ADPFFSource
    variable = d
    free_energy = psi
  []
  [diff]
    type = ADPFFDiffusionRecover
    variable = d
    fracture_toughness = Gc
    regularization_length = l
    normalization_constant = c0
    Fbarprev = deformation_gradient_Fbar
    use_displaced_mesh = false
  []
[]

[Materials]
  [defgrad]
    type = ComputeDeformationGradient
    recover = true
    volumetric_locking_correction = true
    solution = epsol
    recover_mode = polar_decomposition
  []
  [g]
    type = PowerDegradationFunction
    phase_field = d
    property_name = g
    expression = (1-d)^p*(1-eta)+eta
    parameter_names = 'p eta'
    parameter_values = '2 1e-6'
  []
  [fracture_properties]
    type = ADGenericConstantMaterial
    prop_names = 'Gc l'
    prop_values = '${Gc} ${l}'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    property_name = alpha
    expression = 'd'
    phase_field = d
  []
  [psi]
    type = ADDerivativeParsedMaterial
    property_name = psi
    expression = 'alpha*(coal*Gc)/c0/l+g*(psie_corr_active+psip_active)'
    coupled_variables = 'd psip_active psie_corr_active coal'
    material_property_names = 'alpha(d) g(d) Gc c0 l'
    derivative_order = 1
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  line_search = none
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  dt = 1000
  nl_abs_tol = 1e-22
  nl_rel_tol = 1e-6
  automatic_scaling = false
  start_time = ${start_time}

  [Quadrature]
    order = FOURTH
  []
[]

[Outputs]
  print_linear_residuals = false
  [exodus]
    type = Exodus
    file_base = '${out_file}'
    additional_execute_on = 'FAILED'
  []
[]

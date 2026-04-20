# Phase-field fracture sub-app used by reference.i (TET10 variant).

[GlobalParams]
[]

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 1
    ny = 1
    nz = 1
    xmin = 0
    xmax = 3
    ymin = 0
    ymax = 3
    zmin = 0
    zmax = 3
    elem_type = TET10
  []
[]

[Variables]
  [d]
    order = SECOND
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
    type = ADPFFDiffusion
    variable = d
    fracture_toughness = Gc
    regularization_length = l
    normalization_constant = c0
  []
[]

[Materials]
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
  nl_abs_tol = 1e-12
  nl_rel_tol = 1e-7
  automatic_scaling = true

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

# Phase-field fracture sub-app used by reference.i.

[GlobalParams]
[]

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 3
    ny = 3
    nz = 1
    xmin = 0
    xmax = 3
    ymin = 0
    ymax = 3
    zmin = 0
    zmax = 1
    elem_type = HEX8
  []
[]

[Variables]
  [d]
  []
[]

[AuxVariables]
  [psie_corr_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [psip_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [coal]
    order = CONSTANT
    family = MONOMIAL
  []
  [bounds_dummy]
  []
[]

[Bounds]
  [conditional]
    type = ConstantBounds
    bound_value = 0
    bound_type = lower
    variable = bounds_dummy
    bounded_variable = d
  []
  [upper]
    type = ConstantBounds
    variable = bounds_dummy
    bounded_variable = d
    bound_type = upper
    bound_value = 1
  []
[]

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
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'

  dt = 1000
  nl_abs_tol = 1e-12
  nl_rel_tol = 1e-7
  automatic_scaling = true

  [Quadrature]
    order = THIRD
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

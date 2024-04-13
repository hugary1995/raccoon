[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    xmax = 200
    ymax = 1
    nx = ${nx}
    ny = 1
  []
[]

[Variables]
  [d]
    [InitialCondition]
      type = ConstantIC
      boundary = left
      value = 1e-3
    []
  []
[]

[AuxVariables]
  [bounds_dummy]
  []
  [psie_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [disp_x]
  []
  [disp_y]
  []
[]

[Bounds]
  [irreversibility]
    type = VariableOldValueBounds
    variable = bounds_dummy
    bounded_variable = d
    bound_type = lower
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
  [diff]
    type = ADPFFDiffusion
    variable = d
    fracture_toughness = Gc
    regularization_length = l
    normalization_constant = c0
  []
  [source]
    type = ADPFFSource
    variable = d
    free_energy = psi
  []
  [pressure]
    type = ADPFFPressure
    variable = d
    displacements = 'disp_x disp_y'
    pressure = p
    indicator_function = I
  []
[]

[Materials]
  [fracture_properties]
    type = ADGenericConstantMaterial
    prop_names = 'Gc psic l p'
    prop_values = '${Gc} ${psic} ${l} ${p}'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    property_name = alpha
    expression = '2*d-d^2'
    phase_field = d
  []
  [indicator]
    type = ADDerivativeParsedMaterial
    property_name = I
    coupled_variables = 'd'
    expression = 'd^2'
    derivative_order = 1
  []
  [degradation]
    type = RationalDegradationFunction
    property_name = g
    expression = (1-d)^p/((1-d)^p+(Gc/psic*xi/c0/l)*d*(1+a2*d+a2*a3*d^2))*(1-eta)+eta
    phase_field = d
    material_property_names = 'Gc psic xi c0 l '
    parameter_names = 'p a2 a3 eta '
    parameter_values = '2 -0.5 0 1e-6'
  []
  [psi]
    type = ADDerivativeParsedMaterial
    property_name = psi
    expression = 'alpha*Gc/c0/l+g*psie_active'
    coupled_variables = 'd psie_active'
    material_property_names = 'alpha(d) g(d) Gc c0 l'
    derivative_order = 1
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'
  automatic_scaling = true

  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10
[]

[Outputs]
  print_linear_residuals = false
[]

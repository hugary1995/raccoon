[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = '../gold/half_notched_plate_63.msh'
  []
[]

[Variables]
  [d]
  []
[]

[AuxVariables]
  [bounds_dummy]
  []
  [we_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [wp_active]
    order = CONSTANT
    family = MONOMIAL
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
[]

[Materials]
  [fracture_properties]
    type = ADGenericConstantMaterial
    prop_names = 'l Gc psic'
    prop_values = '${l} ${Gc} ${psic}'
  []
  [degradation]
    type = ADDerivativeParsedMaterial
    f_name = g
    args = d
    function = '(1-d)^2/(1+(0.5*Gc/c0/l/psic-1)*d)^2*(1-eta)+eta'
    material_property_names = 'Gc c0 l psic'
    constant_names = 'eta '
    constant_expressions = '5e-3'
    derivative_order = 1
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    phase_field = d
  []
  [psi]
    type = ADDerivativeParsedMaterial
    f_name = psi
    function = 'alpha*Gc/c0/l+g*(we_active+wp_active)'
    args = 'd we_active wp_active'
    material_property_names = 'alpha(d) g(d) Gc c0 l'
    derivative_order = 1
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels '
                        '-snes_type'
  petsc_options_value = 'asm      ilu          200         200                0                     '
                        'vinewtonrsls'
  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06
  automatic_scaling = true
[]

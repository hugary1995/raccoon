[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = '../gold/domain05Coarse.msh'
  []
[]

[Variables]
  [d]
  []
[]

[AuxVariables]
  [bounds_dummy]
  []
  [psie_active]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[Bounds]
  [irreversibility]
    type = VariableOldValueBoundsAux
    variable = bounds_dummy
    bounded_variable = d
    bound_type = lower
  []
  [upper]
    type = ConstantBoundsAux
    variable = bounds_dummy
    bounded_variable = d
    bound_type = upper
    bound_value = 1
  []
[]

[BCs]
  [damageBC]
    type = DirichletBC
    variable = d
    boundary = Hole
    value = 0
  []
[]

[Kernels]
  [diff]
    type = ADPFFDiffusion
    variable = d
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
    prop_names = 'Gc l'
    prop_values = '${Gc} ${l}'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd^2'
    phase_field = d
  []
  [degradation]
    type = PowerDegradationFunction
    f_name = g
    phase_field = d
    parameter_names = 'p eta '
    parameter_values = '2 ${k}'
  []
  [psi]
    type = ADDerivativeParsedMaterial
    f_name = psi
    function = 'alpha*Gc/c0/l+g*psie_active'
    args = 'd psie_active'
    material_property_names = 'alpha(d) g(d) Gc c0 l'
    derivative_order = 1
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels '
                        '-snes_type'
  petsc_options_value = 'asm      lu          200         200                0                     '
                        'vinewtonrsls'
  nl_abs_tol = 1e-06
  nl_rel_tol = 1e-06
  automatic_scaling = true
[]

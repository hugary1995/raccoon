[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = 'gold/domain05.msh'
  []
[]

[Variables]
  [d]
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
  #Testing with irreversibiility
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

[BCs]
  [damageBC]
    type = DirichletBC
    variable = 'd'
    boundary = 'Hole'

    value = 0
    #use_displaced_mesh = true

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
    prop_names = 'Gc psic l'
    prop_values = '${Gc} ${psic} ${l}'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    d = d
  []
  [degradation]
    #Lorentz
    type = RationalDegradationFunction
    f_name = g
    d = d
    parameter_names = 'p a2 a3 eta'
    parameter_values = '2 1 0 1e-09'
  []
  [psi]
    type = ADDerivativeParsedMaterial
    f_name = psi
    function = 'alpha/c0/l+g*we_active/Gc'
    args = 'd we_active'
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

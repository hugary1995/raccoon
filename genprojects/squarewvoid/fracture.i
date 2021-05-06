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
  #[irreversibility]
  #  type = VariableOldValueBoundsAux
  #  variable = 'bounds_dummy'
  #  bounded_variable = 'd'
  #  bound_type = lower
  #[]
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
  [pff_diff]
    type = ADPFFDiffusion
    variable = 'd'
  []
  [pff_barr]
    type = ADPFFBarrier
    variable = 'd'
  []
  [pff_react]
    type = ADPFFReaction
    variable = 'd'
    driving_energy_var = 'E_el_active'
    lag = false
  []
[]

[Materials]
  [bulk]
    type = ADGenericConstantMaterial
    prop_names = 'phase_field_regularization_length energy_release_rate' #critical_fracture_energy'
    prop_values = '${l} ${Gc}' #${psic}'
  []
  [local_dissipation]
    #type = LinearLocalDissipation
    type = QuadraticLocalDissipation
    d = d
  []
  [phase_field_properties]
    type = ADFractureMaterial
    local_dissipation_norm = 8/3
  #  local_dissipation_norm = 2
  []
  [degradation]
    #type = QuadraticDegradation
    type = LorentzDegradation
    d = d
    #residual_degradation = ${k}
    residual_degradation = 1e-09
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

[Problem]
  null_space_dimension = 1
[]

[Mesh]
  [./gmg]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 10
    ny = 10
  [../]
[]

[Variables]
  [./u]
  [../]
[]

[AuxVariables]
  [./aux_u]
    [./InitialCondition]
      type = FunctionIC
      function = '1'
    [../]
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = 'u'
  [../]
[]

[BCs]
  [./left]
    type = NeumannBC
    variable = 'u'
    boundary = 'left'
    value = 5
  [../]
  [./right]
    type = NeumannBC
    variable = 'u'
    boundary = 'right'
    value = -5
  [../]
[]

[Executioner]
  type = SteadyWithNull
  solve_type = 'NEWTON'

  line_search = none

  petsc_options = '-ksp_monitor -snes_monitor -snes_converged_reason'

  petsc_options_iname = '-pc_type -pc_factor_shift_type -pc_factor_shift_amount'
  petsc_options_value = 'lu       NONZERO               1e-10'

  # petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  # petsc_options_value = 'asm      lu           1000        200                0                    '

  # petsc_options_iname = '-pc_type -pc_hypre_type -ksp_pc_side -ksp_norm_type'
  # petsc_options_value = 'hypre    boomeramg      left         preconditioned'

  nl_rel_tol = 1e-14
  nl_abs_tol = 1e-14
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
[]

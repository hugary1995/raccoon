
patchref=4
seed=0
sigma_ts=100

filebase=patchref${patchref}

[Problem]
  type = FEProblem
  # verbose_multiapps = true
  solve = False
[]

[MultiApps]
  [patches]
    type = FullSolveMultiApp
    input_files = patches.i
    cli_args = 'seed=${seed};sigma_ts=${sigma_ts};patchref=${patchref}'
    execute_on = 'INITIAL'
  []
[]

[Transfers]
  [from_patches]
    type = MultiAppGeneralFieldShapeEvaluationTransfer
    from_multi_app = patches
    variable = 'sigma_ts_var'
    source_variable = 'sigma_ts'
    displaced_source_mesh = false
    displaced_target_mesh = false
    execute_on = 'INITIAL'
  []
[]


[Mesh] # h final = 0.25
  [ori]
    type = GeneratedMeshGenerator
    dim = 2
    xmin = 0
    xmax = 100
    ymin = 0
    ymax = 100
    nx = 100
    ny = 100
  []
[]


[Variables]
  [d]
  []
[]

[AuxVariables]
  [sigma_ts_var] # patch
    order = CONSTANT
    family = MONOMIAL
  []
[]


[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package '
  petsc_options_value = 'lu      superlu_dist    '
  automatic_scaling = true

  line_search = bt
  compute_scaling_once = true

  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10

  dt = 1
  dtmin = 1e-5
  end_time = 1
[]

[Outputs]
  [exodus]
    type = Exodus
  []
  # file_base = '${filebase}/${filebase}'
  file_base = '${filebase}'
  print_linear_residuals = false
[]

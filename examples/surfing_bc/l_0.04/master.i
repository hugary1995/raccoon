[Problem]
  solve = false
[]

[Mesh]
  type = FileMesh
  file = 'gold/${n}.msh'
[]

[MultiApps]
  [./mechanical]
    type = TransientMultiApp
    input_files = 'mechanical.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
    sub_cycling = true
    detect_steady_state = true
    steady_state_tol = 1e-3
    cli_args = 'n=${n}'
  [../]
[]

[Transfers]
  [./send_load]
    type = MultiAppScalarToAuxScalarTransfer
    multi_app = mechanical
    direction = to_multiapp
    source_variable = 'a'
    to_aux_scalar = 'a'
  [../]
  [./get_d_ref]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd'
    execute_on = 'TIMESTEP_END'
  [../]
  [./send_d_ref]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = to_multiapp
    source_variable = 'd'
    variable = 'd_ref'
    execute_on = 'TIMESTEP_BEGIN'
  [../]
[]

[AuxVariables]
  [./a]
    family = SCALAR
  [../]
  [./d]
  [../]
[]

[AuxScalarKernels]
  [./a]
    type = FunctionScalarAux
    variable = a
    function = t
    execute_on = 'INITIAL TIMESTEP_BEGIN'
  [../]
[]

[ICs]
  [./d]
    type = CohesiveDamageIC
    variable = d
    d0 = 1.0
    l = 0.04
    x1 = 0
    y1 = 0
    z1 = 0
    x2 = 0.2
    y2 = 0
    z2 = 0
  [../]
[]

[Executioner]
  type = Transient
  dt = 5e-3
  start_time = 0.1
  end_time = 0.6
[]

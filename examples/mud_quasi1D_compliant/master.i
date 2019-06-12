[Problem]
  solve = false
[]

[Mesh]
  type = FileMesh
  file = 'gold/geo.msh'
[]

[MultiApps]
  [./mechanical]
    type = TransientMultiApp
    input_files = 'mechanical.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_END'
    sub_cycling = true
    detect_steady_state = true
    steady_state_tol = 1e-06
  [../]
[]

[Transfers]
  [./load]
    type = MultiAppScalarToAuxScalarTransfer
    multi_app = mechanical
    direction = to_multiapp
    source_variable = 'load'
    to_aux_scalar = 'load'
    execute_on = SAME_AS_MULTIAPP
  [../]
  [./get_d_ref]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd_ref'
    execute_on = 'TIMESTEP_END'
  [../]
  [./send_d_ref]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = to_multiapp
    source_variable = 'd_ref'
    variable = 'd_ref'
    execute_on = 'TIMESTEP_BEGIN'
  [../]
[]

[AuxVariables]
  [./load]
    family = SCALAR
  [../]
  [./d_ref]
  [../]
[]

[AuxScalarKernels]
  [./load]
    type = FunctionScalarAux
    variable = load
    function = t
    execute_on = 'TIMESTEP_BEGIN'
  [../]
[]

[Executioner]
  type = Transient
  dt = 0.001
  end_time = 0.1
[]

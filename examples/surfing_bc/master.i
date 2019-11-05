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
    execute_on = 'TIMESTEP_BEGIN'
    sub_cycling = true
    detect_steady_state = true
    steady_state_tol = 1e-3
  [../]
[]

[Transfers]
  [./get_disp_x]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = from_multiapp
    source_variable = 'disp_x'
    variable = 'disp_x'
  [../]
  [./get_disp_y]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = from_multiapp
    source_variable = 'disp_y'
    variable = 'disp_y'
  [../]
  [./send_load]
    type = MultiAppScalarToAuxScalarTransfer
    multi_app = mechanical
    direction = to_multiapp
    source_variable = 'crack_tip_x'
    to_aux_scalar = 'crack_tip_x'
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
  [./crack_tip_x]
    family = SCALAR
  [../]
  [./d]
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxScalarKernels]
  [./load]
    type = FunctionScalarAux
    variable = crack_tip_x
    function = t-0.5
    execute_on = 'INITIAL TIMESTEP_BEGIN'
  [../]
[]

[Executioner]
  type = Transient
  dt = 1e-3
  end_time = 1
[]

[Outputs]
  exodus = true
[]

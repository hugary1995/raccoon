[Problem]
  solve = false
[]

[Mesh]
  type = FileMesh
  file = 'geo.msh'
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
  [./fracture_energy]
    type = MultiAppPostprocessorTransfer
    multi_app = mechanical
    direction = from_multiapp
    from_postprocessor = 'fracture_energy'
    to_postprocessor = 'fracture_energy'
    execute_on = SAME_AS_MULTIAPP
    reduction_type = average
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

[Postprocessors]
  [./fracture_energy]
    type = Receiver
  [../]
[]

[UserObjects]
  [./termination]
    type = Terminator
    expression = 'fracture_energy > 8.000000E-04'
  [../]
[]

[Executioner]
  type = Transient
  dt = 6.175235E-04
[]

[Outputs]
  csv = true
[]

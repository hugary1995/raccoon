[GlobalParams]
  beta = 0.25
  gamma = 0.5
[]

[Problem]
  solve = false
[]

[Mesh]
  type = FileMesh
  file = 'gold/geo.msh'
[]

[MultiApps]
  [./fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
    sub_cycling = true
    detect_steady_state = true
    steady_state_tol = 0.01
  [../]
[]

[Transfers]
  [./send_load]
    type = MultiAppScalarToAuxScalarTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'load'
    to_aux_scalar = 'load'
  [../]
  [./get_d_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd_ref'
  [../]
  [./get_disp_r]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = 'disp_r'
    variable = 'disp_r'
  [../]
  [./get_disp_z]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = 'disp_z'
    variable = 'disp_z'
  [../]
  [./send_d_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'd_ref'
    variable = 'd_ref'
  [../]

  [./send_disp_r_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'disp_r'
    variable = 'disp_r_ref'
  [../]
  [./send_vel_r_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'vel_r'
    variable = 'vel_r_ref'
  [../]
  [./send_accel_r_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'accel_r'
    variable = 'accel_r_ref'
  [../]
  [./send_disp_z_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'disp_z'
    variable = 'disp_z_ref'
  [../]
  [./send_vel_z_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'vel_z'
    variable = 'vel_z_ref'
  [../]
  [./send_accel_z_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'accel_z'
    variable = 'accel_z_ref'
  [../]
[]

[AuxVariables]
  [./load]
    family = SCALAR
  [../]
  [./d]
  [../]
  [./d_ref]
  [../]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./disp_r]
  [../]
  [./disp_z]
  [../]
  [./vel_r]
  [../]
  [./vel_z]
  [../]
  [./accel_r]
  [../]
  [./accel_z]
  [../]
[]

[AuxKernels]
  [./accel_r]
    type = NewmarkAccelAux
    variable = accel_r
    displacement = disp_r
    velocity = vel_r
    execute_on = 'TIMESTEP_END'
  [../]
  [./vel_r]
    type = NewmarkVelAux
    variable = vel_r
    acceleration = accel_r
    execute_on = 'TIMESTEP_END'
  [../]
  [./accel_z]
    type = NewmarkAccelAux
    variable = accel_z
    displacement = disp_z
    velocity = vel_z
    execute_on = 'TIMESTEP_END'
  [../]
  [./vel_z]
    type = NewmarkVelAux
    variable = vel_z
    acceleration = accel_z
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Functions]
  [./load_fcn]
    type = ParsedFunction
    value = 'if (t<1e-7, 1e20*t, if (t<9e-7, 1e13, if (t<1e-6, 1e14-1e20*t, 0)))'
  [../]
[]

[AuxScalarKernels]
  [./load]
    type = FunctionScalarAux
    variable = load
    function = 'load_fcn'
    execute_on = 'TIMESTEP_BEGIN'
  [../]
[]

[Executioner]
  type = Transient
  dt = 5e-9
  end_time = 1
[]

[Outputs]
  exodus = true
[]

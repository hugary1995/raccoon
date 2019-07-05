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
    execute_on = 'TIMESTEP_END'
    sub_cycling = true
    detect_steady_state = true
    steady_state_tol = 0.1
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
    variable = 'd'
    execute_on = 'TIMESTEP_END'
  [../]
  [./send_d_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'd'
    variable = 'd_ref'
    execute_on = 'TIMESTEP_BEGIN'
  [../]
  [./get_disp_x]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = 'disp_x'
    variable = 'disp_x'
  [../]
  [./get_disp_y]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = 'disp_y'
    variable = 'disp_y'
  [../]
[]

[AuxVariables]
  [./load]
    family = SCALAR
  [../]
  [./d]
  [../]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[ICs]
  [./d]
    type = CohesiveDamageIC
    variable = d
    d0 = 1.5
    l = 2
    x1 = '60 40'
    y1 = '50 30'
    z1 = '0 0'
    x2 = '60 40'
    y2 = '70 50'
    z2 = '0 0'
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
  dt = 1e-3
  end_time = 1e-1
[]

[Outputs]
  exodus = true
[]

E = 2.1e5
nu = 0.3
Gc = 2.7
l = 0.015
psic = 14.88
k = 1e-09
dc = 0.6

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
    steady_state_tol = 1e-6
    cli_args = 'E=${E};nu=${nu};Gc=${Gc};l=${l};psic=${psic};k=${k};dc=${dc}'
  [../]
[]

[Transfers]
  [./send_load]
    type = MultiAppScalarToAuxScalarTransfer
    multi_app = mechanical
    direction = to_multiapp
    source_variable = 'load'
    to_aux_scalar = 'load'
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
  [./load]
    family = SCALAR
  [../]
  [./d]
  [../]
[]

[ICs]
  [./d]
    type = CohesiveDamageIC
    variable = d
    d0 = 1.5
    l = ${l}
    x1 = -0.5
    y1 = 0
    z1 = 0
    x2 = 0
    y2 = 0
    z2 = 0
  [../]
[]

[AuxScalarKernels]
  [./load]
    type = FunctionScalarAux
    variable = 'load'
    function = 't'
    execute_on = 'INITIAL TIMESTEP_BEGIN'
  [../]
[]

[Executioner]
  type = Transient
  dt = 1e-4
  end_time = 2e-2
[]

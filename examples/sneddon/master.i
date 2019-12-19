[Problem]
  solve = false
[]

# [Mesh]
#   type = FileMesh
#   file = 'gold/geo.msh'
# []

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 16
  ny = 16
  xmin = 0
  xmax = 4
  ymin = 0
  ymax = 4
[]

[Adaptivity]
  steps = 1
  marker = box
  max_h_level = 5
  initial_steps = 5
  stop_time = 1.0e-10
  [./Markers]
    [./box]
      bottom_left = '1.6 1.8 0'
      inside = refine
      top_right = '2.4 2.2 0'
      outside = do_nothing
      type = BoxMarker
    [../]
  [../]
[]

[MultiApps]
  [./mechanical]
    type = TransientMultiApp
    input_files = 'mechanical.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
    sub_cycling = true
    detect_steady_state = true
    steady_state_tol = 1e-8
  [../]
[]

[Transfers]
  [./send_pressure]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = to_multiapp
    source_variable = 'p'
    variable = 'p'
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
  [./p]
  [../]
  [./d]
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxKernels]
  [./pressure]
    type = FunctionAux
    variable = 'p'
    function = 't'
    execute_on = 'INITIAL TIMESTEP_BEGIN'
  [../]
[]

[ICs]
  [./d]
    type = BrittleDamageIC
    variable = 'd'
    d0 = 1
    l = 0.02
    x1 = 1.8
    y1 = 2
    z1 = 0
    x2 = 2.2
    y2 = 2
    z2 = 0
  [../]
[]

[Executioner]
  type = Transient
  dt = 1e-4
  end_time = 1e-3
[]

[MultiApps]
  [./thermomechanical]
    type = TransientMultiApp
    input_files = 'thermal_mechanical_dynamic.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Transfers]
  [./get_E_el]
    type = MultiAppMeshFunctionTransfer
    multi_app = thermomechanical
    direction = from_multiapp
    source_variable = 'E_el'
    variable = 'E_el'
  [../]
  [./send_d]
    type = MultiAppMeshFunctionTransfer
    multi_app = thermomechanical
    direction = to_multiapp
    source_variable = 'd'
    variable = 'd'
  [../]
  [./get_d_ref]
    type = MultiAppMeshFunctionTransfer
    multi_app = thermomechanical
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd_ref'
  [../]
[]

[Mesh]
  type = FileMesh
  file = '../gold/geo2.msh'
[]

[Variables]
  [./d]
  [../]
[]

[AuxVariables]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./d_ref]
  [../]
  [./bounds_dummy]
  [../]
  [./Gc]
  [../]
[]

[AuxKernels]
 [./Gc]
   type = FunctionAux
   variable = Gc
   function = Gc
   execute_on = 'INITIAL'
  [../]
[]


[Bounds]
  [./irreversibility]
    type = Irreversibility
    variable = bounds_dummy
    bounded_variable = d
    upper = 1
    lower = d_ref
  [../]
[]

[Kernels]
  [./damage]
    type = PhaseFieldFractureEvolution
    variable = d
    driving_energy_var = E_el
  [../]
[]

[Functions]
  [./Gc]
    type = PiecewiseMultilinear
    data_file = ../gold/Gc2.txt
  [../]
[]

[Materials]
  [./fracture_energy_barrier]
    type = StationaryGenericFunctionMaterial
    prop_names = 'b'
    prop_values = '0.0009'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = Gc
    L = 0.18
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-06
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu vinewtonrsls'

  dt = 5e-9
  end_time = 1.010000E-04

  nl_abs_tol = 1e-06
  nl_rel_tol = 1e-06
[]

[Outputs]
  print_linear_residuals = false
[]

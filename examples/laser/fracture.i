[MultiApps]
  [./thermomechanical]
    type = TransientMultiApp
    input_files = 'thermal_mechanical_dynamic.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
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
  [./get_disp_r]
    type = MultiAppCopyTransfer
    multi_app = thermomechanical
    direction = from_multiapp
    source_variable = 'disp_r'
    variable = 'disp_r'
  [../]
  [./get_disp_z]
    type = MultiAppCopyTransfer
    multi_app = thermomechanical
    direction = from_multiapp
    source_variable = 'disp_z'
    variable = 'disp_z'
  [../]
  [./send_d]
    type = MultiAppMeshFunctionTransfer
    multi_app = thermomechanical
    direction = to_multiapp
    source_variable = 'd'
    variable = 'd'
  [../]
  [./send_load]
    type = MultiAppScalarToAuxScalarTransfer
    multi_app = thermomechanical
    direction = to_multiapp
    source_variable = 'load'
    to_aux_scalar = 'load'
  [../]

  [./send_disp_r_ref]
    type = MultiAppCopyTransfer
    multi_app = thermomechanical
    direction = to_multiapp
    source_variable = 'disp_r_ref'
    variable = 'disp_r_ref'
  [../]
  [./send_vel_r_ref]
    type = MultiAppCopyTransfer
    multi_app = thermomechanical
    direction = to_multiapp
    source_variable = 'vel_r_ref'
    variable = 'vel_r_ref'
  [../]
  [./send_accel_r_ref]
    type = MultiAppCopyTransfer
    multi_app = thermomechanical
    direction = to_multiapp
    source_variable = 'accel_r_ref'
    variable = 'accel_r_ref'
  [../]
  [./send_disp_z_ref]
    type = MultiAppCopyTransfer
    multi_app = thermomechanical
    direction = to_multiapp
    source_variable = 'disp_z_ref'
    variable = 'disp_z_ref'
  [../]
  [./send_vel_z_ref]
    type = MultiAppCopyTransfer
    multi_app = thermomechanical
    direction = to_multiapp
    source_variable = 'vel_z_ref'
    variable = 'vel_z_ref'
  [../]
  [./send_accel_z_ref]
    type = MultiAppCopyTransfer
    multi_app = thermomechanical
    direction = to_multiapp
    source_variable = 'accel_z_ref'
    variable = 'accel_z_ref'
  [../]
[]

[Mesh]
  type = FileMesh
  file = 'gold/geo.msh'
[]

[Variables]
  [./d]
  [../]
[]

[AuxVariables]
  [./load]
    family = SCALAR
  [../]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./d_ref]
  [../]
  [./bounds_dummy]
  [../]
  [./disp_r]
  [../]
  [./disp_z]
  [../]

  [./disp_r_ref]
  [../]
  [./disp_z_ref]
  [../]
  [./vel_r_ref]
  [../]
  [./vel_z_ref]
  [../]
  [./accel_r_ref]
  [../]
  [./accel_z_ref]
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
    data_file = gold/Gc.txt
  [../]
[]

[Materials]
  [./fracture_energy_barrier]
    type = StationaryGenericFunctionMaterial
    prop_names = 'b'
    prop_values = '0.005'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = Gc
    L = 0.1
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-06
  [../]
[]

[Executioner]
  type = TransientSupNorm
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu vinewtonrsls'
  dt = 5e-11

  nl_abs_tol = 1e-06
  nl_rel_tol = 1e-06
[]

[Outputs]
  hide = 'load'
  print_linear_residuals = false
[]

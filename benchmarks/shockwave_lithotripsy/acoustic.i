[MultiApps]
  [./elastodynamic]
    type = TransientMultiApp
    execute_on = 'TIMESTEP_END'
    input_files = 'elastodynamic_exp.i'
  [../]
[]

[Transfers]
  [./from_accel_x]
    type = MultiAppNearestNodeTransfer
    direction = from_multiapp
    multi_app = 'elastodynamic'
    source_variable = 'accel_x'
    variable = 'accel_x'
    source_boundary = 'left_right'
    target_boundary = 'left_right'
    execute_on = 'TIMESTEP_BEGIN'
  [../]
  [./from_accel_y]
    type = MultiAppNearestNodeTransfer
    direction = from_multiapp
    multi_app = 'elastodynamic'
    source_variable = 'accel_y'
    variable = 'accel_y'
    source_boundary = 'top_bottom'
    target_boundary = 'top_bottom'
    execute_on = 'TIMESTEP_BEGIN'
  [../]
  [./to_p]
    type = MultiAppNearestNodeTransfer
    direction = to_multiapp
    multi_app = 'elastodynamic'
    source_variable = 'p'
    variable = 'pre_wave'
    source_boundary = 'inner_BC'
    target_boundary = 'outer_BC'
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Mesh]
   [./fmg]
     type = FileMeshGenerator
     file = 'gold/outermesh.msh'
   [../]
[]

[Variables]
  [./p]
  [../]
[]

[AuxVariables]
  [./vel_p]
  [../]
  [./accel_p]
  [../]
  [./accel_x]
  [../]
  [./accel_y]
  [../]
[]

[Kernels]
  [./inertia_p]
    type = ExplicitInertialForce
    variable = 'p'
  [../]
  [./diff_p]
    type = ADCoefMatDiffusion
    variable = 'p'
    material_property_name = 'Diff'
  [../]
[]

[DiracKernels]
  [./monopole_source]
    type = LithotripsySource
     variable = 'p'
     point = '0.0 2.02 0.0'
     fL = 8.33e-2
     t1 = 0.07
     tRT = 0.01
     tL = 0.8
     tP = 10.0
     p0 = 1e-6
     d1 = 9
     upcoeff = 12.2189
     downcoeff = 0.9404
     diffusivity = 'Diff'
  [../]
[]

[BCs]
  [./top]
    type = CoupledNeumannBC
    variable = 'p'
    boundary = 'top2'
    coupled_var = 'accel_y'
  [../]
  [./left]
    type = CoupledNeumannBC
    variable = 'p'
    boundary = 'left2'
    coupled_var = 'accel_x'
  [../]
  [./right]
    type = CoupledNeumannBC
    variable = 'p'
    boundary = 'right2'
    coupled_var = 'accel_x'
  [../]
  [./bottom]
    type = CoupledNeumannBC
    variable = 'p'
    boundary = 'bottom2'
    coupled_var = 'accel_y'
  [../]
[]

[Materials]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density Diff'
    prop_values = '444.44 1000'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      31                  preonly       lu           1'
  start_time = 0
  end_time = 3.0
  l_max_its = 50
  nl_max_its = 20
  l_tol = 1e-9
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8
  dt = 1e-3

  automatic_scaling = true
[]

[Outputs]
  [./exodus]
    type = Exodus
    #interval = 1
    file_base = AcousticOut
  [../]
[]

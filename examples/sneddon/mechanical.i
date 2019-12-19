[MultiApps]
  [./fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
  [../]
[]

[Transfers]
  [./send_disp_x]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'disp_x'
    variable = 'disp_x'
  [../]
  [./send_disp_y]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'disp_y'
    variable = 'disp_y'
  [../]
  [./send_p]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'p'
    variable = 'p'
  [../]
  [./send_d_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'd_ref'
    variable = 'd_ref'
  [../]
  [./get_d]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd'
  [../]
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

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./d]
  [../]
  [./d_ref]
  [../]
  [./p]
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./stress_yy]
    type = RankTwoAux
    variable = stress_yy
    rank_two_tensor = stress
    index_i = 1
    index_j = 1
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    displacements = 'disp_x disp_y'
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    displacements = 'disp_x disp_y'
  [../]
  [./pressure_x]
    type = PressurizedCrack
    variable = disp_x
    d = d
    p = p
    component = 0
  [../]
  [./pressure_y]
    type = PressurizedCrack
    variable = disp_y
    d = d
    p = p
    component = 1
  [../]
[]

[BCs]
  [./yfix]
    type = PresetBC
    variable = disp_y
    boundary = 'top bottom left right'
    value = 0
  [../]
  [./xfix]
    type = PresetBC
    variable = disp_x
    boundary = 'top bottom left right'
    value = 0
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1
    poissons_ratio = 0.2
  [../]
  [./strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_NoSplit
    d = d
    history = false
  [../]
  [./fracture_properties]
    type = GenericConstantMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length b'
    prop_values = '1 0.0221 0'
  [../]
  [./local_dissipation]
    type = QuadraticLocalDissipation
    d = d
  [../]
  [./phase_field_properties]
    type = FractureMaterial
    local_dissipation_norm = 2
  [../]
  [./degradation]
    type = QuadraticDegradation
    d = d
    residual_degradation = 1e-12
  [../]
[]

[Executioner]
  type = TransientSupNorm
  solve_type = 'NEWTON'
  # petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  # petsc_options_value = 'lu       superlu_dist'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  petsc_options_value = 'asm      ilu          1000        200                0                    '
  dt = 1e-6

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  automatic_scaling = true
  compute_scaling_once = false
[]

[Outputs]
  print_linear_residuals = false
  exodus = true
[]

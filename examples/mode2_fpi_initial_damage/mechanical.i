[MultiApps]
  [./fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
  [../]
[]

[Transfers]
  [./send_E_el]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'E_el'
    variable = 'E_el'
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

[Mesh]
  type = FileMesh
  file = 'gold/geo.msh'
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
  [./load]
    family = SCALAR
  [../]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./fx]
  [../]
[]

[AuxKernels]
  [./E_el]
    type = MaterialRealAux
    property = 'E_el'
    variable = 'E_el'
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    displacements = 'disp_x disp_y'
    save_in = fx
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    displacements = 'disp_x disp_y'
  [../]
[]

[BCs]
  [./xdisp]
    type = ScalarDirichletBC
    variable = disp_x
    boundary = 'top'
    scalar_var = load
  [../]
  [./yfix]
    type = DirichletBC
    variable = disp_y
    boundary = 'top bottom left right'
    value = 0
  [../]
  [./xfix]
    type = DirichletBC
    variable = disp_x
    boundary = 'bottom'
    value = 0
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.1e5
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_StrainSpectral
    d = d
    d_crit = 0.8
    history = false
  [../]
  [./fracture_energy_barrier]
    type = StationaryGenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length b'
    prop_values = '2.7 0.015 14.88'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-09
  [../]
[]

[Executioner]
  type = TransientSupNorm
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'
  # petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  # petsc_options_value = 'asm      ilu          1000        200                0                    '
  dt = 1e-6

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  automatic_scaling = true
[]

[Postprocessors]
  [./Fx]
    type = SideIntegralVariablePostprocessor
    variable = fx
    boundary = 'top'
  [../]
[]

[Outputs]
  print_linear_residuals = false
  [./csv]
    type = CSV
    delimiter = ' '
  [../]
  [./console]
    type = Console
    hide = 'load Fx'
  [../]
[]

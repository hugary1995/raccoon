[MultiApps]
  [./fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
    cli_args = 'n=${n}'
  [../]
[]

[Transfers]
  [./send_D]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'D'
    variable = 'D'
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
  file = 'gold/${n}.msh'
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
  [./a]
    order = FIRST
    family = SCALAR
  [../]
  [./fx]
  [../]
  [./fy]
  [../]
  [./alpha]
    family = MONOMIAL
  [../]
  [./D]
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./alpha]
    type = MaterialRealAux
    property = 'alpha'
    variable = 'alpha'
    execute_on = 'TIMESTEP_END'
  [../]
  [./D]
    type = MaterialRealAux
    property = 'fracture_driving_energy'
    variable = 'D'
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    displacements = 'disp_x disp_y'
    save_in = 'fx'
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    displacements = 'disp_x disp_y'
    save_in = 'fy'
  [../]
[]

[BCs]
  [./x]
    type = ModeISurfingDirichletBC
    variable = disp_x
    boundary = 'top left right'
    component = 0
    E = 2.1e5
    nu = 0.3
    Gc = 2.7
    crack_tip_x = a
    crack_tip_y = 0
  [../]
  [./y]
    type = ModeISurfingDirichletBC
    variable = disp_y
    boundary = 'top left right'
    component = 1
    E = 2.1e5
    nu = 0.3
    Gc = 2.7
    crack_tip_x = a
    crack_tip_y = 0
  [../]
  [./symmetry]
    type = DirichletBC
    variable = disp_y
    value = 0
    boundary = 'bottom'
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.1e5
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = GreenStrain
    displacements = 'disp_x disp_y'
  [../]
  [./elastic]
    type = CNHDegradedPK1ElastoPlasticStress_VolDev
    d = d
    history = false
    W0 = 0
    linear_hardening_coefficient = 1e5
    yield_stress = 1000
  [../]
  [./fracture_properties]
    type = GenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length b'
    prop_values = '2.7 0.04 14.88'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./phase_field_properties]
    type = FractureMaterial
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-09
  [../]
[]

[Postprocessors]
  [./fracture_energy]
    type = FractureEnergy
    d = d
  [../]
  [./surface_area]
    type = FractureSurfaceArea
    d = d
  [../]
  [./J]
    type = PhaseFieldJIntegral
    J_direction = '1 0 0'
    boundary = 'top left right'
    displacements = 'disp_x disp_y'
    elastic_energy_name = 'degraded_elastic_energy'
  [../]
  [./external_energy]
    type = ExternalEnergy
    boundary = 'top bottom left right'
    displacements = 'disp_x disp_y'
    forces = 'fx fy'
  [../]
  [./plastic_work]
    type = ElementIntegralMaterialProperty
    mat_prop = 'degraded_plastic_work'
  [../]
  [./elastic_energy]
    type = ElementIntegralMaterialProperty
    mat_prop = 'degraded_elastic_energy'
  [../]
[]

[Executioner]
  type = TransientSupNorm
  solve_type = 'NEWTON'
  # petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  # petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  petsc_options_value = 'asm      ilu          1000        200                0'
  dt = 1e-4

  nl_abs_tol = 1e-12
  nl_rel_tol = 1e-08

  automatic_scaling = true
  compute_scaling_once = false
[]

[Outputs]
  file_base = '${n}'
  print_linear_residuals = false
  hide = 'a'
  csv = true
  exodus = true
[]

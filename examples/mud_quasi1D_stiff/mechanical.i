[GlobalParams]
  displacements = 'disp_x disp_y'
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
  [../]
[]

[Transfers]
  [./disp_x]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'disp_x'
    variable = 'disp_x'
    execute_on = SAME_AS_MULTIAPP
  [../]
  [./disp_y]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'disp_y'
    variable = 'disp_y'
    execute_on = SAME_AS_MULTIAPP
  [../]
  [./d]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd'
    execute_on = SAME_AS_MULTIAPP
  [../]
  [./load]
    type = MultiAppScalarToAuxScalarTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'load'
    to_aux_scalar = 'load'
    execute_on = SAME_AS_MULTIAPP
  [../]
[]

[Modules]
  [./KLExpansion]
    file_name = 'gold/kl_info.txt'
    field_name = 'E_crit'
    field_distribution = GAMMA
    perturbation = CUSTOM
    custom_Gaussian_weights = '0.537667139546100 1.83388501459509 -2.25884686100365 0.862173320368121 0.318765239858981 -1.30768829630527 -0.433592022305684'
    mean = 4e-4
    CV = 0.3
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
  [./load]
    family = SCALAR
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
  [../]
[]

[BCs]
  [./boundary_xdisp]
    type = DirichletBC
    variable = disp_x
    boundary = 'bottom'
    value = 0
  [../]
  [./boundary_ydisp]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  [../]
  [./Periodic]
    [./disp_x]
      variable = disp_x
      primary = 'left'
      secondary = 'right'
      translation = '20 0 0'
    [../]
    [./disp_y]
      variable = disp_y
      primary = 'left'
      secondary = 'right'
      translation = '20 0 0'
    [../]
  [../]
[]

[Materials]
  [./eigen_strain]
    type = ComputeEigenstrainFromScalarInitialStress
    initial_stress_xx = load
    eigenstrain_name = is
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = 0.2
    youngs_modulus = 4
  [../]
  [./strain]
    type = ADComputeSmallStrain
    eigenstrain_names = is
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_NoSplit
    history = false
  [../]
  [./fracture_energy_barrier]
    type = GenericFunctionMaterial
    prop_names = 'b'
    prop_values = 'E_crit'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = 8e-4
    L = 0.1
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
  [../]
[]

[Executioner]
  type = TransientSupNorm
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  dt = 1e-5

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-06
[]

[Outputs]
  hide = 'load'
  exodus = true
  print_linear_residuals = false
[]

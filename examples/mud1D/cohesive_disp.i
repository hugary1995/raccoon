[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Modules]
  [./KLExpansion]
    file_name = 'gold/kl_info.txt'
    field_name = 'E_crit'
    field_distribution = GAMMA
    perturbation = CUSTOM
    custom_Gaussian_weights = '0.537667139546100	1.83388501459509	-2.25884686100365	0.862173320368121	0.318765239858981	-1.30768829630527	-0.433592022305684'
    mean = 4e-4
    CV = 0.3
  [../]
[]

[MultiApps]
  [./damage]
    type = TransientMultiApp
    input_files = 'cohesive_damage.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Transfers]
  [./disp_x]
    type = MultiAppCopyTransfer
    multi_app = damage
    direction = to_multiapp
    source_variable = 'disp_x'
    variable = 'disp_x'
  [../]
  [./disp_y]
    type = MultiAppCopyTransfer
    multi_app = damage
    direction = to_multiapp
    source_variable = 'disp_y'
    variable = 'disp_y'
  [../]
  [./d]
    type = MultiAppCopyTransfer
    multi_app = damage
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd'
  [../]
[]

[Mesh]
  type = FileMesh
  file = 'gold/film.msh'
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

[Materials]
  [./eigen_strain]
    type = ComputeEigenstrainFromFunctionEigenstress
    initial_stress = 't 0 0 0 0 0 0 0 0'
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
    history = true
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
    L = 0.15
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
  [../]
[]

[BCs]
  [./boundary_xdisp]
    type = DirichletBC
    variable = disp_x
    boundary = 'bottom left right'
    value = 0
  [../]
  [./boundary_ydisp]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  dt = 1e-3

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-06

  picard_max_its = 100
  picard_abs_tol = 1e-06
  picard_rel_tol = 1e-1
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

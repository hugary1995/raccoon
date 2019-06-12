[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = FileMesh
  file = 'gold/geo.msh'
[]

[Modules]
  [./KLExpansion]
    file_name = 'gold/kl_info.txt'
    field_name = 'E_crit'
    field_distribution = GAMMA
    perturbation = CUSTOM
    custom_Gaussian_weights = '0.537667139546100 1.83388501459509 -2.25884686100365 0.862173320368121 0.318765239858981 -1.30768829630527 -0.433592022305684'
    mean = 4e-4
    CV = 0.05
  [../]
[]

[Variables]
  [./d]
  [../]
[]

[AuxVariables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./d_ref]
  [../]
  [./bounds_dummy]
  [../]
  [./load]
    family = SCALAR
  [../]
[]

[Bounds]
  [./irreversibility]
    type = Irreversibility
    variable = bounds_dummy
    bounded_var = d
    upper = 1
    lower = d_ref
  [../]
[]

[Kernels]
  [./damage]
    type = PhaseFieldFractureEvolution
    variable = d
    driving_energy_name = E_el
  [../]
[]

[Materials]
  [./eigen_strain]
    type = ComputeEigenstrainFromScalarInitialStress
    initial_stress_xx = load
    eigenstrain_name = is
  [../]
  [./elasticity_tensor_film]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = 0.2
    youngs_modulus = 4
    block = 'film'
  [../]
  [./elasticity_tensor_substrate]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = 0.2
    youngs_modulus = 800
    block = 'substrate'
  [../]
  [./strain]
    type = ADComputeSmallStrain
    eigenstrain_names = is
  [../]
  [./stress_film]
    type = SmallStrainDegradedPK2Stress_NoSplit
    history = false
    block = 'film'
  [../]
  [./stress_substrate]
    type = SmallStrainPK2Stress
    block = 'substrate'
  [../]
  [./fracture_energy_barrier_film]
    type = GenericFunctionMaterial
    prop_names = 'b'
    prop_values = 'E_crit'
    block = 'film'
  [../]
  [./fracture_energy_barrier_substrate]
    type = GenericConstantMaterial
    prop_names = 'b'
    prop_values = '1'
    block = 'substrate'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties_film]
    type = FractureMaterial
    Gc = 8e-4
    L = 0.1
    local_dissipation_norm = 8/3
    block = 'film'
  [../]
  [./fracture_properties_substrate]
    type = FractureMaterial
    Gc = 8e-4
    L = 0.1
    local_dissipation_norm = 8/3
    block = 'substrate'
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu vinewtonrsls'
  dt = 1e-5

  nl_abs_tol = 1e-06
  nl_rel_tol = 1e-06
[]

[Outputs]
  hide = 'load'
  print_linear_residuals = false
[]

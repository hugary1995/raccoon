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

[Mesh]
  type = FileMesh
  file = gold/film.msh
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
  # [./bounds_dummy]
  # [../]
[]

# [Bounds]
#   [./irreversibility]
#     type = Irreversibility
#     variable = bounds_dummy
#     bounded_var = d
#     execute_on = 'LINEAR TIMESTEP_BEGIN'
#   [../]
# []

[Kernels]
  [./damage]
    type = PhaseFieldFractureEvolution
    variable = d
    driving_energy_name = E_el
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

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  dt = 1e-3

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-06
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

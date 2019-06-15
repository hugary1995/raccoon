[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = FileMesh
  file = 'geo.msh'
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
  [./nonnegtivity]
    type = BoundsAux
    variable = bounds_dummy
    bounded_variable = d
    upper = 1
    lower = 0
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
    poissons_ratio = 2.000000E-01
    youngs_modulus = 4.166667E+00
    block = 'film'
  [../]
  [./elasticity_tensor_substrate]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = 2.000000E-01
    youngs_modulus = 4.166667E+01
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
    type = GenericConstantMaterial
    prop_names = 'b'
    prop_values = '5.812010E-04'
    block = 'film'
  [../]
  [./fracture_energy_barrier_substrate]
    type = GenericConstantMaterial
    prop_names = 'b'
    prop_values = '5.812010E+00'
    block = 'substrate'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties_film]
    type = FractureMaterial
    Gc = 8.000000E-04
    L = 6.309573E-02
    local_dissipation_norm = 8/3
    block = 'film'
  [../]
  [./fracture_properties_substrate]
    type = FractureMaterial
    Gc = 8.000000E-04
    L = 6.309573E-02
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
  dt = 5.023145E-06

  nl_abs_tol = 1e-06
  nl_rel_tol = 1e-06
[]

[Outputs]
  hide = 'load'
  print_linear_residuals = false
[]

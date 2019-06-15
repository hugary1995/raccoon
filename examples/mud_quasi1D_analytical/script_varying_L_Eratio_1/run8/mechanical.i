[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = FileMesh
  file = 'geo.msh'
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
  [./d_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'd_ref'
    variable = 'd_ref'
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
      translation = '1.258925E+01 0 0'
    [../]
    [./disp_y]
      variable = disp_y
      primary = 'left'
      secondary = 'right'
      translation = '1.258925E+01 0 0'
    [../]
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
    youngs_modulus = 4.166667E+00
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
    prop_values = '4.048456E-04'
    block = 'film'
  [../]
  [./fracture_energy_barrier_substrate]
    type = GenericConstantMaterial
    prop_names = 'b'
    prop_values = '4.048456E+00'
    block = 'substrate'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties_film]
    type = FractureMaterial
    Gc = 8.000000E-04
    L = 1.258925E-01
    local_dissipation_norm = 8/3
    block = 'film'
  [../]
  [./fracture_properties_substrate]
    type = FractureMaterial
    Gc = 8.000000E-04
    L = 1.258925E-01
    local_dissipation_norm = 8/3
    block = 'substrate'
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
  [../]
[]

[Postprocessors]
  [./fracture_energy]
    type = FractureEnergy
    d = d
  [../]
[]

[Executioner]
  type = TransientSupNorm
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  dt = 4.121224E-06

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-06
[]

[Outputs]
  hide = 'load fracture_energy'
  exodus = true
  print_linear_residuals = false
[]

[Functions]
  [./psic]
    type = PiecewiseMultilinear
    data_file = '../gold/psic_L_3_rho_0_sample_1.txt'
  [../]
  [./Gc]
    type = PiecewiseMultilinear
    data_file = '../gold/Gc_L_3_rho_0_sample_1.txt'
  [../]
[]

[Mesh]
  type = FileMesh
  file = '../gold/geo.msh'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
[]

[AuxVariables]
  [./d]
  [../]
  [./load]
    family = SCALAR
  [../]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
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
    displacements = 'disp_x disp_y disp_z'
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    displacements = 'disp_x disp_y disp_z'
  [../]
  [./solid_z]
    type = ADStressDivergenceTensors
    variable = disp_z
    component = 2
    displacements = 'disp_x disp_y disp_z'
  [../]
[]

[BCs]
  [./x_fix]
    type = PresetBC
    variable = disp_x
    value = 0
    boundary = 'bottom'
  [../]
  [./y_fix]
    type = PresetBC
    variable = disp_y
    value = 0
    boundary = 'bottom'
  [../]
  [./z_fix]
    type = PresetBC
    variable = disp_z
    value = 0
    boundary = 'bottom'
  [../]
[]

[Materials]
  [./eigen_strain]
    type = ComputeEigenstrainFromScalarInitialStress
    initial_stress_xx = load
    initial_stress_yy = load
    initial_stress_zz = load
    eigenstrain_name = is
  [../]
  [./strain]
    type = ADComputeSmallStrain
    eigenstrain_names = is
    displacements = 'disp_x disp_y disp_z'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-06
  [../]

  [./elasticity_tensor_film]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = 0.2
    youngs_modulus = 4
  [../]
  [./stress_film]
    type = SmallStrainDegradedPK2Stress_NoSplit
    d = d
    history = false
  [../]
  [./fracture_energy_barrier_film]
    type = GenericFunctionMaterial
    prop_names = 'b'
    prop_values = 'psic'
  [../]
  [./fracture_properties_film]
    type = FractureMaterial
    Gc = 'Gc'
    L = 0.5
    local_dissipation_norm = 8/3
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  # petsc_options_iname = '-pc_type'
  # petsc_options_value = 'lu'
  petsc_options_iname = '-pc_type -sub_ksp_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  petsc_options_value = 'asm      preonly       ilu          200         200                0                    '
  # petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  # petsc_options_value = 'asm      31                  preonly       ilu          2'
  # petsc_options_iname = '-pc_type -mat_mffd_err -pc_factor_shift_type -pc_factor_shift_amount'
  # petsc_options_value = 'ilu      1e-5          NONZERO               1e-15'
  dt = 1e-4

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-06
[]

[Outputs]
  hide = 'load'
  print_linear_residuals = false
[]

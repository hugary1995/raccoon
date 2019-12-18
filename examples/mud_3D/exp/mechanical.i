[Functions]
  [./Gc]
    type = PiecewiseMultilinear
    data_file = '../Gc_exp.txt'
  [../]
  [./psic]
    type = PiecewiseMultilinear
    data_file = '../psic_exp.txt'
  [../]
[]

[Mesh]
  type = FileMesh
  file = '../gold/2D.msh'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./strain_zz]
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
    displacements = 'disp_x disp_y'
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    displacements = 'disp_x disp_y'
  [../]
  [./plane_stress]
    type = ADWeakPlaneStress
    variable = strain_zz
    displacements = 'disp_x disp_y'
  [../]
  [./react_x]
    type = DegradedCoefReaction
    variable = disp_x
    coefficient = 0.1
  [../]
  [./react_y]
    type = DegradedCoefReaction
    variable = disp_y
    coefficient = 0.1
  [../]
[]

[BCs]
  [./x_pin]
    type = DirichletBC
    variable = disp_x
    value = 0
    boundary = 'boundary'
  [../]
  [./y_pin]
    type = DirichletBC
    variable = disp_y
    value = 0
    boundary = 'boundary'
  [../]
[]

[Materials]
  [./eigen_strain]
    type = ComputeEigenstrainFromScalarInitialStress
    initial_stress_xx = load
    initial_stress_yy = load
    eigenstrain_name = is
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = 0.2
    youngs_modulus = 4
  [../]
  [./strain]
    type = ADComputePlaneSmallStrain
    out_of_plane_strain = strain_zz
    eigenstrain_names = is
    displacements = 'disp_x disp_y'
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_NoSplit
    d = d
    history = false
  [../]
  [./fracture_energy_barrier]
    type = StationaryGenericFunctionMaterial
    prop_names = 'energy_release_rate b'
    prop_values = 'Gc psic'
  [../]
  [./length_scale]
    type = GenericConstantMaterial
    prop_names = 'phase_field_regularization_length'
    prop_values = '0.5'
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
    residual_degradation = 1e-06
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  # petsc_options_iname = '-pc_type'
  # petsc_options_value = 'lu'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  petsc_options_value = 'asm      ilu          200         200                0                    '
  dt = 1e-4

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-06

  automatic_scaling = true
[]

[Outputs]
  hide = 'load'
  print_linear_residuals = false
[]

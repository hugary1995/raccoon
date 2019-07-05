[GlobalParams]
  displacements = 'disp_x disp_y'
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
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
  [../]
  [./plane_stress]
    type = ADWeakPlaneStress
    variable = strain_zz
  [../]
[]

[BCs]
  [./Periodic]
    [./x_left_right]
      variable = disp_x
      primary = 'left'
      secondary = 'right'
      translation = '100 0 0'
    [../]
    [./y_left_right]
      variable = disp_y
      primary = 'left'
      secondary = 'right'
      translation = '100 0 0'
    [../]
    [./x_top_bottom]
      variable = disp_x
      primary = 'top'
      secondary = 'bottom'
      translation = '0 -100 0'
    [../]
    [./y_top_bottom]
      variable = disp_y
      primary = 'top'
      secondary = 'bottom'
      translation = '0 -100 0'
    [../]
  [../]
  [./x_pin]
    type = DirichletBC
    variable = disp_x
    value = 0
    boundary = 'center'
  [../]
  [./y_pin]
    type = DirichletBC
    variable = disp_y
    value = 0
    boundary = 'center pin'
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
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_NoSplit
    d = d
    # d_crit = 0.6
    history = false
    output_properties = stress
    outputs = exodus
  [../]
  [./fracture_energy_barrier]
    type = GenericFunctionMaterial
    prop_names = 'b'
    prop_values = '3e-5'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = 8e-4
    L = 0.5
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
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  # petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  # petsc_options_value = 'asm      ilu          200         200                0                    '
  dt = 1e-4

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-06

  automatic_scaling = true
[]

[Outputs]
  hide = 'load'
  exodus = true
  print_linear_residuals = false
[]

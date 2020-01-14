[Problem]
  type = FixedPointProblem
[]

[Mesh]
  [./square]
    type = GeneratedMeshGenerator
    dim = 2
    elem_type = QUAD
    nx = 200
    ny = 200
    xmax = 100
    ymax = 100
  [../]
  [./center]
    type = ExtraNodesetGenerator
    input = 'square'
    coord = '50 50'
    new_boundary = 'center'
  [../]
  [./pin]
    type = ExtraNodesetGenerator
    input = 'center'
    coord = '55 50'
    new_boundary = 'pin'
  [../]
[]

[Functions]
  [./Gc]
    type = PiecewiseMultilinear
    data_file = 'gold/Gc.txt'
  [../]
  [./psic]
    type = PiecewiseMultilinear
    data_file = 'gold/psic.txt'
  [../]
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./strain_zz]
  [../]
  [./d]
  [../]
[]

[AuxVariables]
  [./Gc]
  [../]
  [./psic]
  [../]
  [./bounds_dummy]
  [../]
[]

[AuxKernels]
  [./Gc]
    type = FunctionAux
    variable = 'Gc'
    function = 'Gc'
    execute_on = 'INITIAL'
  [../]
  [./psic]
    type = FunctionAux
    variable = 'psic'
    function = 'psic'
    execute_on = 'INITIAL'
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    component = 0
    displacements = 'disp_x disp_y'
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
    displacements = 'disp_x disp_y'
  [../]
  [./plane_stress]
    type = ADWeakPlaneStress
    variable = 'strain_zz'
    displacements = 'disp_x disp_y'
  [../]
  [./react_x]
    type = CoefMatReaction
    variable = 'disp_x'
    coefficient = 0.1
    material_property_name = 'g'
  [../]
  [./react_y]
    type = CoefMatReaction
    variable = 'disp_y'
    coefficient = 0.1
    material_property_name = 'g'
  [../]
  [./pf_diff]
    type = PhaseFieldFractureEvolutionDiffusion
    variable = 'd'
  [../]
  [./pf_react]
    type = PhaseFieldFractureEvolutionReaction
    variable = 'd'
    driving_energy_mat = 'E_driving'
    lag = true
  [../]
[]

[BCs]
  [./Periodic]
    [./x_left_right]
      variable = 'disp_x'
      primary = 'left'
      secondary = 'right'
      translation = '100 0 0'
    [../]
    [./y_left_right]
      variable = 'disp_y'
      primary = 'left'
      secondary = 'right'
      translation = '100 0 0'
    [../]
    [./d_left_right]
      variable = 'd'
      primary = 'left'
      secondary = 'right'
      translation = '100 0 0'
    [../]
    [./x_top_bottom]
      variable = 'disp_x'
      primary = 'top'
      secondary = 'bottom'
      translation = '0 -100 0'
    [../]
    [./y_top_bottom]
      variable = 'disp_y'
      primary = 'top'
      secondary = 'bottom'
      translation = '0 -100 0'
    [../]
    [./d_top_bottom]
      variable = 'd'
      primary = 'top'
      secondary = 'bottom'
      translation = '0 -100 0'
    [../]
  [../]
  [./x_pin]
    type = DirichletBC
    variable = 'disp_x'
    value = 0
    boundary = 'center'
  [../]
  [./y_pin]
    type = DirichletBC
    variable = 'disp_y'
    value = 0
    boundary = 'center pin'
  [../]
[]

[Bounds]
  [./irr]
    type = Irreversibility
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    lower = 'd'
    upper = 1
    lag = true
  [../]
[]

[Materials]
  [./eigen_strain]
    type = ComputeEigenstrainFromFunctionEigenstress
    initial_stress = 't 0 0 0 t 0 0 0 0'
    eigenstrain_name = 'is'
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = 0.2
    youngs_modulus = 4
  [../]
  [./strain]
    type = ADComputePlaneSmallStrain
    out_of_plane_strain = 'strain_zz'
    eigenstrain_names = 'is'
    displacements = 'disp_x disp_y'
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_NoSplit
    d = 'd'
    d_crit = 0.6
    history = false
  [../]
  [./interface_energy]
    type = ThinFilmInterfaceEnergy
    coef = 0.1
    displacements = 'disp_x disp_y'
  [../]
  [./fracture_driving_energy]
    type = ADSumMaterial
    sum_prop_name = 'E_driving'
    prop_names = 'E_el E_int'
  [../]
  [./fracture_energy_barrier]
    type = StationaryGenericFunctionMaterial
    prop_names = 'energy_release_rate critical_fracture_energy'
    prop_values = 'Gc psic'
  [../]
  [./length_scale]
    type = GenericConstantMaterial
    prop_names = 'phase_field_regularization_length'
    prop_values = '0.5'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = 'd'
  [../]
  [./fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = 'd'
    residual_degradation = 1e-06
  [../]
[]

[Executioner]
  type = FixedPointTransient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type'
  petsc_options_value = 'asm      ilu          200        200                0                     vinewtonrsls'
  dt = 1e-3
  end_time = 0.22

  nl_abs_tol = 1e-12
  nl_rel_tol = 1e-08

  fp_max_its = 10
  fp_tol = 1e-03

  automatic_scaling = true
  compute_scaling_once = false
[]

[Outputs]
  print_linear_residuals = false
  [./exodus]
    type = Exodus
    file_base = 'visualize'
    hide = 'bounds_dummy'
  [../]
  [./console]
    type = Console
    outlier_variable_norms = false
  [../]
[]

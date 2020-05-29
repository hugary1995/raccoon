[Problem]
  type = FixedPointProblem
[]

[UserObjects]
  [./E_driving]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'E_driving'
  [../]
[]

[Mesh]
  type = FileMesh
  file = 'gold/fields.e'
  uniform_refine = 2
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
    initial_from_file_var = 'Gc'
  [../]
  [./psic]
    initial_from_file_var = 'psic'
  [../]
  [./bounds_dummy]
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
    type = ADCoefMatReaction
    variable = 'disp_x'
    coefficient = 0.1
    prop_names = 'g'
  [../]
  [./react_y]
    type = ADCoefMatReaction
    variable = 'disp_y'
    coefficient = 0.1
    prop_names = 'g'
  [../]
  [./pf_diff]
    type = ADPFFDiffusion
    variable = 'd'
  [../]
  [./pf_barr]
    type = ADPFFBarrier
    variable = 'd'
  [../]
  [./pf_react]
    type = ADPFFReaction
    variable = 'd'
    driving_energy_uo = 'E_driving'
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
[]

[Bounds]
  [./irr]
    type = VariableOldValueBoundsAux
    variable = 'bounds_dummy'
    bound_type = lower
    bounded_variable = 'd'
  [../]
  [./upper]
    type = ConstantBoundsAux
    variable = 'bounds_dummy'
    bound_type = upper
    bounded_variable = 'd'
    bound_value = 1
  [../]
[]

[Materials]
  [./eigen_strain]
    type = ADComputeEigenstrainFromFunctionEigenstress
    eigen_stress = 't 0 0 0 t 0 0 0 0'
    eigenstrain_name = 'is'
  [../]
  [./elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
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
    type = SmallStrainDegradedElasticPK2Stress_NoSplit
    d = 'd'
    d_crit = 0.6
  [../]
  [./interface_energy]
    type = ThinFilmInterfaceEnergy
    coef = 0.1
    displacements = 'disp_x disp_y'
  [../]
  [./fracture_driving_energy]
    type = ADSumRealMaterial
    sum_prop_name = 'E_driving'
    prop_names = 'E_el_active E_int'
  [../]
  [./energy_release_rate]
    type = ParsedMaterial
    f_name = 'energy_release_rate'
    args = 'Gc'
    function = 'Gc'
  [../]
  [./critial_fracture_energy]
    type = ParsedMaterial
    f_name = 'critical_fracture_energy'
    args = 'psic'
    function = 'psic'
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
    constant_in_time = false
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
  petsc_options_value = 'asm      ilu          200         200                0                     vinewtonrsls'
  dt = 1e-3
  end_time = 0.04

  nl_abs_tol = 1e-8
  nl_rel_tol = 1e-6

  fp_max_its = 10
  fp_tol = 1e-03

  automatic_scaling = true
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

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = 'gold/fields.e'
    use_for_exodus_restart = true
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [strain_zz]
  []
[]

[AuxVariables]
  [d]
  []
  [E_el_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [Gc]
    initial_from_file_var = 'Gc'
  []
  [psic]
    initial_from_file_var = 'psic'
  []
[]

[AuxKernels]
  [E_driving]
    type = ADMaterialRealAux
    variable = 'E_el_active'
    property = 'E_el_active'
    execute_on = 'TIMESTEP_END'
  []
[]

[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    component = 0
    displacements = 'disp_x disp_y'
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
    displacements = 'disp_x disp_y'
  []
  [plane_stress]
    type = ADWeakPlaneStress
    variable = 'strain_zz'
    displacements = 'disp_x disp_y'
  []
  [react_x]
    type = ADCoefMatReaction
    variable = 'disp_x'
    coefficient = 0.1
    prop_names = 'g'
  []
  [react_y]
    type = ADCoefMatReaction
    variable = 'disp_y'
    coefficient = 0.1
    prop_names = 'g'
  []
[]

[BCs]
  [disp_x_fix]
    type = DirichletBC
    variable = disp_x
    value = 0
    boundary = 'top bottom left right'
  []
  [disp_y_fix]
    type = DirichletBC
    variable = disp_y
    value = 0
    boundary = 'top bottom left right'
  []
  # There is an issue with periodic BC due to ghosting functors.
  # This issue should be fixed pretty soon.
  # [Periodic]
  #   [x]
  #     variable = disp_x
  #     auto_direction = 'x y'
  #   []
  #   [y]
  #     variable = disp_y
  #     auto_direction = 'x y'
  #   []
  # []
[]

[Materials]
  [eigen_strain]
    type = ADComputeEigenstrainFromFunctionEigenstress
    eigen_stress = 't 0 0 0 t 0 0 0 0'
    eigenstrain_name = 'es'
  []
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    poissons_ratio = 0.2
    youngs_modulus = 4
  []
  [strain]
    type = ADComputePlaneSmallStrain
    out_of_plane_strain = 'strain_zz'
    eigenstrain_names = 'es'
    displacements = 'disp_x disp_y'
  []
  [stress]
    type = SmallStrainDegradedElasticPK2Stress_NoSplit
    d = 'd'
    d_crit = 0.6
  []

  [energy_release_rate]
    type = ADParsedMaterial
    f_name = 'Gc'
    args = 'Gc'
    function = 'Gc'
  []
  [critial_fracture_energy]
    type = ADParsedMaterial
    f_name = 'psic'
    args = 'psic'
    function = 'psic'
  []
  [mobility]
    type = ADParsedMaterial
    f_name = L
    material_property_names = 'Gc c0 l'
    function = 'Gc/c0/l'
  []
  [kappa]
    type = ADParsedMaterial
    f_name = kappa
    material_property_names = 'l'
    function = '2*l^2'
  []
  [length_scale]
    type = ADGenericConstantMaterial
    prop_names = 'l'
    prop_values = '1'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    d = d
    initial_derivative_name = xi
    normalization_constant_name = c0
  []
  [degradation]
    type = RationalDegradationFunction
    f_name = g
    d = d
    parameter_names = 'p a2 a3 eta '
    parameter_values = '2 1 0 1e-6'
    material_property_names = 'Gc psic xi c0 l '
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'
  dt = 1e-3
  end_time = 0.22

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  automatic_scaling = true
[]

[Outputs]
  print_linear_residuals = false
[]

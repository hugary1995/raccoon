E = 4
nu = 0.2
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

l = 0.5
c = 0.1

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    cli_args = 'l=${l}'
  []
[]

[Transfers]
  [send_psie_active]
    type = MultiAppMeshFunctionTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = psie_active
    variable = psie_active
  []
  [send_psii_active]
    type = MultiAppMeshFunctionTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = psii_active
    variable = psii_active
  []
  [get_d]
    type = MultiAppMeshFunctionTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = d
    variable = d
  []
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = 'gold/fields_refined.e'
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
  [Gc]
    initial_from_file_var = Gc
  []
  [psic]
    initial_from_file_var = psic
  []
[]

[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
  []
  [plane_stress]
    type = ADWeakPlaneStress
    variable = strain_zz
  []
  [react_x]
    type = ADCoefMatReaction
    variable = disp_x
    coefficient = ${c}
    prop_names = 'g'
  []
  [react_y]
    type = ADCoefMatReaction
    variable = disp_y
    coefficient = ${c}
    prop_names = 'g'
  []
[]

[BCs]
  [Periodic]
    [x]
      variable = disp_x
      auto_direction = 'x y'
    []
    [y]
      variable = disp_y
      auto_direction = 'x y'
    []
  []
[]

[Materials]
  # fracture
  [fracture_toughness]
    type = ADParsedMaterial
    f_name = Gc
    args = Gc
    function = 'Gc'
  []
  [critial_fracture_energy]
    type = ADParsedMaterial
    f_name = psic
    args = psic
    function = 'psic'
  []
  [bulk_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G l'
    prop_values = '${K} ${G} ${l}'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    phase_field = d
  []
  [degradation]
    type = RationalDegradationFunction
    f_name = g
    function = (1-d)^p/((1-d)^p+(Gc/psic*xi/c0/l)*d*(1+a2*d+a2*a3*d^2))*(1-eta)+eta
    phase_field = d
    parameter_names = 'p a2 a3 eta '
    parameter_values = '2 1 0 1e-6'
    material_property_names = 'Gc psic xi c0 l '
  []
  # elasticity
  [eigen_strain]
    type = ComputeEigenstrainFromFunctionInitialStress
    initial_stress = 't t 0'
    eigenstrain_name = 'es'
    bulk_modulus = K
    shear_modulus = G
  []
  [strain]
    type = ADComputePlaneSmallStrain
    out_of_plane_strain = strain_zz
    eigenstrain_names = 'es'
  []
  [elasticity]
    type = SmallDeformationIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    decomposition = NONE
    output_properties = 'psie_active'
    outputs = exodus
  []
  [stress]
    type = ComputeSmallDeformationStress
    elasticity_model = elasticity
  []
  [interfacial_energy]
    type = ThinFilmInterfaceEnergyDensity
    phase_field = d
    shear_lag_coef = ${c}
    output_properties = 'psii_active'
    outputs = exodus
  []
[]

[Postprocessors]
  [delta_d]
    type = SolutionChangeNorm
    variable = d
    outputs = none
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'
  dt = 0.0005
  end_time = 0.22

  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08

  automatic_scaling = true

  picard_max_its = 10
  picard_custom_pp = delta_d
  disable_picard_residual_norm_check = true
  custom_abs_tol = 1e-03
  custom_rel_tol = 1e-03
  accept_on_max_picard_iteration = true
[]

[Outputs]
  print_linear_residuals = false
  exodus = true
[]

[MultiApps]
  [./fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
    cli_args = 'Gc=${Gc};l=${l};psic=${psic};k=${k}'
  [../]
[]

[Transfers]
  [./send_E_el]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'E_el'
    variable = 'E_el'
  [../]
  [./send_d_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'd_ref'
    variable = 'd_ref'
  [../]
  [./get_d]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd'
  [../]
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
[]

[AuxVariables]
  [./d]
  [../]
  [./d_ref]
  [../]
  [./load]
    family = SCALAR
  [../]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./fy]
  [../]
[]

[AuxKernels]
  [./E_el]
    type = ADMaterialRealAux
    variable = 'E_el'
    property = 'E_el_active'
    execute_on = 'TIMESTEP_END'
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
    save_in = 'fy'
  [../]
[]

[BCs]
  [./ydisp]
    type = ScalarDirichletBC
    variable = 'disp_y'
    boundary = 'top'
    scalar_var = 'load'
  [../]
  [./xfix]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'top bottom'
    value = 0
  [../]
  [./yfix]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'bottom'
    value = 0
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = ${E}
    poissons_ratio = ${nu}
  [../]
  [./strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  [../]
  [./stress]
    type = SmallStrainDegradedElasticPK2Stress_StrainSpectral
    d = 'd'
    d_crit = ${dc}
  [../]
  [./fracture_properties]
    type = GenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length critical_fracture_energy'
    prop_values = '${Gc} ${l} ${psic}'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./phase_field_properties]
    type = FractureMaterial
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = ${k}
  [../]
[]

[Executioner]
  type = TransientSubcycling
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'
  dt = 1e-6

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  automatic_scaling = true
  compute_scaling_once = false
[]

[Postprocessors]
  [./Fy]
    type = NodalSum
    variable = 'fy'
    boundary = 'top'
  [../]
[]

[Outputs]
  print_linear_residuals = false
  [./csv]
    type = CSV
    delimiter = ' '
    file_base = 'force_displacement'
    time_column = false
  [../]
  [./exodus]
    type = Exodus
    file_base = 'visualize'
  [../]
  [./console]
    type = Console
    hide = 'load Fy'
  [../]
[]

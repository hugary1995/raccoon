E = 2.1e5
nu = 0.3
Gc = 2.7
l = 0.015
k = 1e-09

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
    cli_args = 'Gc=${Gc};l=${l};k=${k}'
  []
[]

[Transfers]
  [send_E_el]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'E_el_active'
    variable = 'E_el_active'
  []
  [get_d]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd'
  []
[]

[Mesh]
  type = FileMesh
  file = 'gold/geo0.msh'
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
[]

[AuxVariables]
  [d]
  []
  [E_el_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [fx]
  []
[]

[AuxKernels]
  [E_el_active]
    type = ADMaterialRealAux
    variable = 'E_el_active'
    property = 'E_el_active'
  []
[]

[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    component = 0
    displacements = 'disp_x disp_y'
    save_in = 'fx'
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
    displacements = 'disp_x disp_y'
  []
[]

[BCs]
  [xdisp]
    type = FunctionDirichletBC
    variable = 'disp_x'
    boundary = 'top'
    function = 't'
  []
  [yfix]
    type = DirichletBC
    variable = 'disp_y'
    # boundary = 'top bottom left right'
    boundary = 'top bottom'
    value = 0
  []
  [xfix]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'bottom'
    value = 0
  []
[]

[Materials]
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = ${E}
    poissons_ratio = ${nu}
  []
  [strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  []
  [stress]
    type = SmallStrainDegradedElasticPK2Stress_StrainVolDev
    d = 'd'
  []
  [fracture_properties]
    type = GenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length critical_fracture_energy'
    prop_values = '${Gc} ${l} ${psic}'
  []
  [local_dissipation]
    type = QuadraticLocalDissipation
    d = d
  []
  [phase_field_properties]
    type = FractureMaterial
    local_dissipation_norm = 2
  []
  [degradation]
    type = QuadraticDegradation
    d = d
    residual_degradation = ${k}
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'
  dt = 1e-4
  end_time = 2e-2

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  automatic_scaling = true

  picard_max_its = 100
  picard_abs_tol = 1e-08
  picard_rel_tol = 1e-06
[]

[Postprocessors]
  [Fx]
    type = NodalSum
    variable = 'fx'
    boundary = 'top'
  []
[]

[Outputs]
  print_linear_residuals = false
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
  [csv]
    type = CSV
    delimiter = ' '
    file_base = 'force_displacement'
    time_column = false
  []
  [exodus]
    type = Exodus
    file_base = 'visualize'
  []
  [console]
    type = Console
    hide = 'Fx'
  []
[]

E = 2.1e5
nu = 0.3
Gc = 2.7
l = 0.015
psic = 14.88
k = 1e-06
dc = 0.6

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
    cli_args = 'Gc=${Gc};l=${l};psic=${psic};k=${k}'
  []
[]

[Transfers]
  [send_E_el_active]
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
  [square]
    type = GeneratedMeshGenerator
    dim = 2
    xmin = -0.5
    xmax = 0.5
    ymin = -0.5
    ymax = 0.5
    nx = 26
    ny = 26
  []
[]

[Adaptivity]
  marker = 'box'
  max_h_level = 3
  initial_steps = 3
  [Markers]
    [box]
      type = BoxMarker
      bottom_left = '-0.5 -0.04 0'
      top_right = '0.5 0.04 0'
      inside = refine
      outside = do_nothing
    []
  []
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
  [fy]
  []
[]

[AuxKernels]
  [E_el]
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
    save_in = 'fy'
  []
[]

[BCs]
  [ydisp]
    type = FunctionDirichletBC
    variable = 'disp_y'
    boundary = 'top'
    function = 't'
    preset = false
  []
  [xfix]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'top bottom'
    value = 0
  []
  [yfix]
    type = DirichletBC
    variable = 'disp_y'
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
    type = SmallStrainDegradedElasticPK2Stress_StrainSpectral
    d = 'd'
    d_crit = ${dc}
  []
  [fracture_properties]
    type = ADGenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length critical_fracture_energy'
    prop_values = '${Gc} ${l} ${psic}'
  []
  [local_dissipation]
    type = LinearLocalDissipation
    d = d
  []
  [phase_field_properties]
    type = ADFractureMaterial
    local_dissipation_norm = 8/3
  []
  [degradation]
    type = LorentzDegradation
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
  end_time = 6e-3
  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08

  automatic_scaling = true

  picard_max_its = 100
  picard_abs_tol = 1e-50
  picard_rel_tol = 1e-03
  accept_on_max_picard_iteration = true
[]

[Postprocessors]
  [Fy]
    type = NodalSum
    variable = 'fy'
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
  []
  [exodus]
    type = Exodus
    file_base = 'visualize'
  []
[]

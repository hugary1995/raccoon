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
  [./fx]
  [../]
[]

[AuxKernels]
  [./E_el]
    type = MaterialRealAux
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
    save_in = 'fx'
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
    displacements = 'disp_x disp_y'
  [../]
[]

[BCs]
  [./xdisp]
    type = ScalarDirichletBC
    variable = 'disp_x'
    boundary = 'top'
    scalar_var = 'load'
  [../]
  [./yfix]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'top bottom left right'
    value = 0
  [../]
  [./xfix]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'bottom'
    value = 0
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = ${E}
    poissons_ratio = ${nu}
  [../]
  [./strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_StrainSpectral
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
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  petsc_options_value = 'asm      ilu          200         200                0                    '
  dt = 1e-6

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  automatic_scaling = true
  compute_scaling_once = false
[]

[Postprocessors]
  [./Fx]
    type = NodalSum
    variable = 'fx'
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
    hide = 'load Fx'
  [../]
[]

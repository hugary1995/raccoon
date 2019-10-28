[MultiApps]
  [./fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
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
  [./crack_tip_x]
    family = SCALAR
  [../]
  [./crack_tip_y]
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
    save_in = fy
  [../]
[]

[BCs]
  [./x]
    type = ModeISurfingBC
    variable = disp_x
    boundary = 'top bottom left right'
    component = 0
    E = 2.1e5
    nu = 0.3
    Gc = 2.7
    crack_tip_x = crack_tip_x
    crack_tip_y = crack_tip_y
  [../]
  [./y]
    type = ModeISurfingBC
    variable = disp_y
    boundary = 'top bottom left right'
    component = 1
    E = 2.1e5
    nu = 0.3
    Gc = 2.7
    crack_tip_x = crack_tip_x
    crack_tip_y = crack_tip_y
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.1e5
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_StrainSpectral
    d = d
    d_crit = 0.6
    history = false
  [../]
  [./fracture_energy_barrier]
    type = GenericFunctionMaterial
    prop_names = 'b'
    prop_values = '14.88'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = 2.7
    L = 0.015
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-09
  [../]
[]

[Executioner]
  type = TransientSupNorm
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'
  # petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  # petsc_options_value = 'asm      ilu          1000        200                0                    '
  dt = 1e-5

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  automatic_scaling = true
[]

[Outputs]
  print_linear_residuals = false
  # [./csv]
  #   type = CSV
  #   delimiter = ' '
  # [../]
  [./console]
    type = Console
    hide = 'crack_tip_x crack_tip_y'
  [../]
[]

[Mesh]
  type = FileMesh
  file = 'gold/geo.msh'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./d]
  [../]
[]

[AuxVariables]
  [./bounds_dummy]
  [../]
[]

[Bounds]
  [./irreversibility]
    type = Irreversibility
    variable = bounds_dummy
    bounded_variable = d
    upper = 1
    lower = d
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
  [../]
  [./pff]
    type = PhaseFieldFractureEvolution
    variable = d
    driving_energy_name = E_el
  [../]
[]

[BCs]
  [./ydisp]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'top'
    function = 't'
  [../]
  [./xfix]
    type = DirichletBC
    variable = disp_x
    boundary = 'top bottom'
    value = 0
  [../]
  [./yfix]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
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
    d_crit = 2.0
    history = false
  [../]
  [./bulk]
    type = GenericConstantMaterial
    prop_names = 'phase_field_regularization_length energy_release_rate b'
    prop_values = '0.015 2.7 14.88'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-09
  [../]
[]

[Preconditioning]
  [./FSP]
    type = FSP
    petsc_options = '-ksp_converged_reason -snes_converged_reason'
    topsplit = 'ud'
    [./ud]
      splitting = 'u d'
      splitting_type = 'schur'
      petsc_options_iname = '-pc_fieldsplit_schur_fact_type -pc_fieldsplit_schur_precondition'
      petsc_options_value = 'full                           selfp'
    [../]
    [./u]
      vars = 'disp_x disp_y'
      petsc_options_iname = '-pc_type -pc_hypre_type -ksp_atol'
      petsc_options_value = 'hypre    boomeramg      1e-5'
    [../]
    [./d]
      vars = 'd'
      petsc_options_iname = '-pc_type -ksp_type -ksp_atol'
      petsc_options_value = 'jacobi   gmres     1e-5'
    [../]
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  # petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type'
  # petsc_options_value = 'asm      ilu          1000        200                0                     vinewtonrsls'
  # petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  # petsc_options_value = 'asm      ilu          1000        200                0                    '
  dt = 1e-4
  end_time = 6e-3

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  # automatic_scaling = true
  # compute_scaling_once = false
[]

[Outputs]
  print_linear_residuals = false
  exodus = true
[]

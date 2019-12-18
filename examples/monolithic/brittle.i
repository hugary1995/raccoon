[Problem]
  type = FixedPointProblem
  fp_tag_name = 'lag'
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
  [./d]
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    displacements = 'disp_x disp_y'
    # vector_tags = 'lag'
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    displacements = 'disp_x disp_y'
    # vector_tags = 'lag'
  [../]
  [./diffusion]
    type = PhaseFieldFractureEvolutionDiffusion
    variable = d
    driving_energy_name = 'E_el'
    lag_driving_energy = false
    # vector_tags = 'lag'
  [../]
  [./reaction]
    type = PhaseFieldFractureEvolutionReaction
    variable = d
    driving_energy_name = 'E_el'
    lag_driving_energy = false
    vector_tags = 'lag'
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
    prop_values = '0.015 2.7 0'
  [../]
  [./local_dissipation]
    type = QuadraticLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 2
  [../]
  [./degradation]
    type = QuadraticDegradation
    d = d
    residual_degradation = 1e-09
  [../]
[]

[Executioner]
  type = FixedPointTransient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  petsc_options_value = 'asm      ilu          1000        200                0                    '
  dt = 1e-4
  end_time = 6e-3

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  automatic_scaling = true
  compute_scaling_once = false

  # fp_max_its = 100
  # fp_tol = 1e-06
  # accept_on_max_fp_iteration = true

  fp_max_its = 100
[]

[Outputs]
  print_linear_residuals = false
  exodus = true
[]

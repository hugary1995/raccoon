[Problem]
  type = FixedPointProblem
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

[AuxVariables]
  [./bounds_dummy]
  [../]
  [./fy]
  [../]
[]

[UserObjects]
  [./E_driving]
    type = FPIMaterialPropertyUserObject
    mat_prop = 'E_el_active'
  [../]
[]

[Bounds]
  [./irreversibility]
    type = VariableOldValueBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = lower
  [../]
  [./upper]
    type = ConstantBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = upper
    bound_value = 1
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
  [./pff_diff]
    type = PhaseFieldFractureEvolutionDiffusion
    variable = 'd'
  [../]
  [./pff_barr]
    type = PhaseFieldFractureEvolutionBarrier
    variable = 'd'
  [../]
  [./pff_react]
    type = PhaseFieldFractureEvolutionReaction
    variable = 'd'
    driving_energy_uo = 'E_driving'
    lag = false
  [../]
[]

[BCs]
  [./ydisp]
    type = FunctionDirichletBC
    variable = 'disp_y'
    boundary = 'top'
    function = 't'
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
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.1e5
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  [../]
  [./stress]
    type = SmallStrainDegradedElasticPK2Stress_StrainSpectral
    d = 'd'
    d_crit = 2.0
  [../]
  [./bulk]
    type = GenericConstantMaterial
    prop_names = 'phase_field_regularization_length energy_release_rate critical_fracture_energy'
    prop_values = '0.015 2.7 14.88'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = 'd'
  [../]
  [./fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = 'd'
    residual_degradation = 1e-09
  [../]
[]

[Postprocessors]
  [./Fy]
    type = NodalSum
    variable = 'fy'
    boundary = 'top'
  [../]
[]

[Executioner]
  type = FixedPointTransient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type'
  petsc_options_value = 'asm      ilu          200         200                0                     vinewtonrsls'
  dt = 1e-4
  end_time = 6e-3

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  automatic_scaling = true
  compute_scaling_once = false

  fp_max_its = 100
  fp_tol = 1e-06
  accept_on_max_fp_iteration = true
[]

[Outputs]
  print_linear_residuals = false
  [./csv]
    type = CSV
    delimiter = ' '
    file_base = 'force_displacement'
  [../]
  [./exodus]
    type = Exodus
    file_base = 'visualize'
  [../]
  [./console]
    type = Console
    outlier_variable_norms = false
  [../]
[]

E = 2.1e5
nu = 0.3
Gc = 2.7
l = 0.015
psic = 14.88
k = 1e-6
dc = 0.6

[Problem]
  type = FixedPointProblem
[]

[Mesh]
  [./square]
    type = GeneratedMeshGenerator
    dim = 2
    xmin = -0.5
    xmax = 0.5
    ymin = -0.5
    ymax = 0.5
    nx = 26
    ny = 26
  [../]
[]

[Adaptivity]
  steps = 1
  marker = 'box'
  max_h_level = 3
  initial_steps = 3
  stop_time = 1.0e-10
  [./Markers]
    [./box]
      type = BoxMarker
      bottom_left = '-0.5 -0.04 0'
      inside = refine
      top_right = '0.5 0.04 0'
      outside = do_nothing
    [../]
  [../]
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
    type = Irreversibility
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    upper = 1
    lower = 'd'
    lag = true
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
  [./pff_barrier]
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

[ICs]
  [./d]
    type = CohesiveDamageIC
    variable = d
    d0 = 1.0
    l = ${l}
    x1 = -0.5
    y1 = 0
    z1 = 0
    x2 = 0
    y2 = 0
    z2 = 0
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
    type = SmallStrainDegradedElasticPK2Stress_StrainSpectral
    d = 'd'
    d_crit = ${dc}
  [../]
  [./bulk]
    type = GenericConstantMaterial
    prop_names = 'phase_field_regularization_length energy_release_rate critical_fracture_energy'
    prop_values = '${l} ${Gc} ${psic}'
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
    residual_degradation = ${k}
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
    file_base = 'visualize_tet2'
  [../]
  [./console]
    type = Console
    outlier_variable_norms = false
  [../]
[]

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
  [./fx]
  [../]
  [./grad_d_x]
    order = 
    family = MONOMIAL
  [../]
  [./grad_d_y]
    order = CONSTANT
    family = MONOMIAL
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

[AuxKernels]
  [./grad_d_x]
    type = VariableGradientComponent
    variable = 'grad_d_x'
    component = 'x'
    gradient_variable = 'd'
    execute_on = 'TIMESTEP_END'
  [../]
  [./grad_d_y]
    type = VariableGradientComponent
    variable = 'grad_d_y'
    component ='y'
    gradient_variable = 'd'
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
  [./pff_diff]
    type = ADPFFDiffusion
    variable = 'd'
  [../]
  [./pff_barrier]
    type = ADPFFBarrier
    variable = 'd'
  [../]
  [./pff_react]
    type = ADPFFReaction
    variable = 'd'
    driving_energy_uo = 'E_driving'
    lag = false
  [../]
[]

[BCs]
  [./xdisp]
    type = FunctionDirichletBC
    variable = 'disp_x'
    boundary = 'top'
    function = 't'
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
  [./Fx]
    type = NodalSum
    variable = 'fx'
    boundary = 'top'
  [../]
[]

[Executioner]
  type = FixedPointTransient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type'
  petsc_options_value = 'asm      ilu          200         200                0                     vinewtonrsls'
  dt = 1e-4
  end_time = 2e-2

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

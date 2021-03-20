E = 1
nu = 0.2
Gc = 1
l = 0.1
psic = 0
k = 0
dc = 1
#p_cr = 1.28758e-2 (sneddon)

[Problem]
  type = FixedPointProblem
[]

[Mesh]
 [./gmg]
  type = GeneratedMeshGenerator
  dim = 2
  nx = 101
  ny = 101
  xmin = -1
  xmax = 1
  ymin = -1
  ymax = 1
 [../]
 [./middle_nodes]
  type = BoundingBoxNodeSetGenerator
  input = 'gmg'
  new_boundary = middle_nodes
  bottom_left = '-0.0101 -0.501 0'
  top_right = '0.0101 0.501 0'
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
[]

[UserObjects]
  [./E_driving]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'E_el_active'
  [../]
  [./pressure_uo]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'p'
  [../]
  [./g]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'g'
  [../]
[]

# [Bounds]
#   [./irreversibility]
#     type = VariableOldValueBoundsAux
#     variable = 'bounds_dummy'
#     bounded_variable = 'd'
#     bound_type = lower
#   [../]
#   [./upper]
#     type = ConstantBoundsAux
#     variable = 'bounds_dummy'
#     bounded_variable = 'd'
#     bound_type = upper
#     bound_value = 1
#   [../]
# []

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
  [../]
  [./pressure_body_force_x]
    type = ADPressurizedCrack
    variable = 'disp_x'
    d = 'd'
    pressure_mat = 'p'
    component = 0
    lag = true
  [../]
  [./pressure_body_force_y]
    type = ADPressurizedCrack
    variable = 'disp_y'
    d = 'd'
    pressure_mat = 'p'
    component = 1
    lag = true
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
  [./pff_pressure]
    type = ADPFFPressure
    variable = 'd'
    pressure_uo = 'pressure_uo'
    displacements = 'disp_x disp_y'
    lag = true
  [../]
[]

[BCs]
  [./xfix]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'top bottom left right'
    value = 0
  [../]
  [./yfix]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'top bottom left right'
    value = 0
  [../]
  [.dfixed]
    type = DirichletBC
    variable = 'd'
    boundary = 'middle_nodes'
    value = 1
  [../]
[]

# [ICs]
#   [./d]
#     type = BrittleDamageIC
#     variable = d
#     d0 = 1.0
#     l = ${l}
#     bandwidth_multiplier = 100
#     x1 = '-0.2 -0.2'
#     y1 = '0 3e-3'
#     z1 = '0 0'
#     x2 = '0.2 0.2'
#     y2 = '0 3e-3'
#     z2 = '0 0'
#   [../]
# []

[Materials]
  [./pressure]
    type = ADGenericFunctionMaterial
    prop_names = 'p'
    prop_values = '-1e-2'
  [../]
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
    type = SmallStrainDegradedElasticPK2Stress_NoSplit
    d = 'd'
    d_crit = ${dc}
    degradation_uo = 'g'
  [../]
  [./bulk]
    type = GenericConstantMaterial
    prop_names = 'phase_field_regularization_length critical_fracture_energy energy_release_rate'
    prop_values = '${l} ${psic} ${Gc}'
  [../]
  [./local_dissipation]
    type = QuadraticLocalDissipation
    d = 'd'
  [../]
  [./fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 2
  [../]
  [./degradation]
    type = QuadraticDegradation
    d = 'd'
    residual_degradation = ${k}
  [../]
  # [./Gc_function]
  #   type = GenericFunctionMaterial
  #   prop_names = 'energy_release_rate'
  #   prop_values = '${Gc}*(1+x^2)'
  # [../]
[]

[Executioner]
  type = FixedPointTransient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type'
  petsc_options_value = 'lu      ilu          200         200                0                     vinewtonrsls'
  dt = 1e-3
  end_time = 3e-3

  nl_abs_tol = 1e-12
  nl_rel_tol = 1e-12
  nl_max_its = 150

  automatic_scaling = true
  compute_scaling_once = false

  fp_max_its = 1
  fp_tol = 1e-06
  accept_on_max_fp_iteration = true
[]

[Outputs]
  print_linear_residuals = false
  [./exodus]
    type = Exodus
    file_base = 'compare_geosx_2d'
  [../]
  [./console]
    type = Console
    outlier_variable_norms = false
  [../]
[]

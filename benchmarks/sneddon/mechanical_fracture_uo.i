r = 5
E = 1
nu = 0.2
Gc = 1e-3
l = 0.02
psic = 1
k = 1e-12
xi = 1

[Problem]
  type = FixedPointProblem
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    xmin = 0
    xmax = 4
    ymin = 2
    ymax = 4
    nx = 16
    ny = 16
  []
[]

[Adaptivity]
  marker = box
  initial_steps = ${r}
  max_h_level = ${r}
  [Markers]
    [box]
      type = BoxMarker
      bottom_left = '1.6 1.8 0'
      top_right = '2.4 2.1 0'
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
  [d]
  []
[]

[AuxVariables]
  [bounds_dummy]
  []
[]

[UserObjects]
  [E_driving]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'E_el_active'
  []
  [pressure_uo]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'p'
  []
[]

[Bounds]
  [irreversibility]
    type = VariableOldValueBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = lower
  []
  [upper]
    type = ConstantBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = upper
    bound_value = 1
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
  []
  [pressure_body_force_x]
    type = ADPressurizedCrack
    variable = 'disp_x'
    d = 'd'
    pressure_mat = 'p'
    component = 0
    xi = ${xi}
  []
  [pressure_body_force_y]
    type = ADPressurizedCrack
    variable = 'disp_y'
    d = 'd'
    pressure_mat = 'p'
    component = 1
    xi = ${xi}
  []
  [pff_diff]
    type = ADPFFDiffusion
    variable = 'd'
  []
  [pff_barrier]
    type = ADPFFBarrier
    variable = 'd'
  []
  [pff_react]
    type = ADPFFReaction
    variable = 'd'
    driving_energy_uo = 'E_driving'
    lag = false
  []
  [pff_pressure]
    type = ADPFFPressure
    variable = 'd'
    pressure_uo = 'pressure_uo'
    displacements = 'disp_x disp_y'
    xi = ${xi}
  []
[]

[BCs]
  [xfix]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'top bottom left right'
    value = 0
  []
  [yfix]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'top bottom left right'
    value = 0
  []
[]

[ICs]
  [initial_crack]
    type = BoundingBoxIC
    variable = 'd'
    inside = 1
    x1 = 1.819999
    y1 = 1.999999
    x2 = 2.180001
    y2 = 2.000001
  []
[]

[Materials]
  [pressure]
    type = ADGenericFunctionMaterial
    prop_names = 'p'
    prop_values = 't'
  []
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
    type = SmallStrainDegradedElasticPK2Stress_NoSplit
    d = 'd'
  []
  [bulk]
    type = GenericConstantMaterial
    prop_names = 'phase_field_regularization_length energy_release_rate critical_fracture_energy'
    prop_values = '${l} ${Gc} ${psic}'
  []
  [local_dissipation]
    type = QuadraticLocalDissipation
    d = 'd'
  []
  [fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 2
  []
  [degradation]
    type = QuadraticDegradation
    d = 'd'
    residual_degradation = ${k}
  []
[]

[Postprocessors]
  [fracture_energy]
    type = FractureEnergy
    d = 'd'
  []
[]

[Executioner]
  type = FixedPointTransient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_type_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                vinewtonrsls'

  dt = 1e-3

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  automatic_scaling = true

  fp_max_its = 100
  fp_tol = 1e-03
  accept_on_max_fp_iteration = true
[]

[Outputs]
  print_linear_residuals = false
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
  [exodus]
    type = Exodus
    file_base = 'l_${l}_xi_${xi}_r_${r}/visualize'
  []
  [console]
    type = Console
    outlier_variable_norms = false
  []
[]

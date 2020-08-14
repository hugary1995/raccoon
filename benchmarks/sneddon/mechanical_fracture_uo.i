E = 1
nu = 0.0
Gc = 1
l = 0.1
psic = 0
k = 1e-12

[GlobalParams]
  # out_of_plane_strain = 'strain_zz'
[]

[Problem]
  type = FixedPointProblem
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    xmax = 4
    ymax = 4
    nx = 16
    ny = 16
  []
  # uniform_refine = 3
[]

[Adaptivity]
  steps = 1
  marker = box
  max_h_level = 6
  initial_steps = 6
  stop_time = 1.0e-10
  [Markers]
    [box]
      bottom_left = '1.6 1.8 0'
      inside = refine
      top_right = '2.4 2.2 0'
      outside = do_nothing
      type = BoxMarker
    []
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  # [strain_zz]
  # []
  [d]
  []
[]

[AuxVariables]
  [bounds_dummy]
  []
  [cod]
    order = FIRST
    family = MONOMIAL
  []
[]

[AuxKernels]
  [cod]
    type = PFFCrackOpening
    variable = 'cod'
    displacements = 'disp_x disp_y'
    d = 'd'
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
  # [strain_zz]
  #   type = ADWeakPlaneStress
  #   variable = 'strain_zz'
  # []
  [pressure_body_force_x]
    type = ADPressurizedCrack
    variable = 'disp_x'
    d = 'd'
    pressure_mat = 'p'
    component = 0
  []
  [pressure_body_force_y]
    type = ADPressurizedCrack
    variable = 'disp_y'
    d = 'd'
    pressure_mat = 'p'
    component = 1
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
  [d]
    type = BrittleDamageIC
    variable = d
    d0 = 1.0
    l = ${l}
    x1 = 1.8
    y1 = 2
    z1 = 0
    x2 = 2.2
    y2 = 2
    z2 = 0
  []
[]

[Materials]
  [pressure]
    type = ADGenericFunctionMaterial
    prop_names = 'p'
    prop_values = '1e-3'
  []
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = ${E}
    poissons_ratio = ${nu}
  []
  [strain]
    type = ADComputePlaneSmallStrain
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

[Executioner]
  type = FixedPointTransient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_type_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                vinewtonrsls'

  num_steps = 1

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  automatic_scaling = true

  fp_max_its = 100
  fp_tol = 1e-06
  accept_on_max_fp_iteration = true
[]

[Outputs]
  print_linear_residuals = false
  csv = false
  [exodus]
    type = Exodus
    file_base = 'visualize'
  []
  [console]
    type = Console
    outlier_variable_norms = false
  []
[]

[VectorPostprocessors]
  [cod1]
    type = LineValueSampler
    start_point = '1.5 4 0'
    end_point = '1.5 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod2]
    type = LineValueSampler
    start_point = '1.55 4 0'
    end_point = '1.55 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod3]
    type = LineValueSampler
    start_point = '1.6 4 0'
    end_point = '1.6 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod4]
    type = LineValueSampler
    start_point = '1.65 4 0'
    end_point = '1.65 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod5]
    type = LineValueSampler
    start_point = '1.7 4 0'
    end_point = '1.7 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod6]
    type = LineValueSampler
    start_point = '1.71 4 0'
    end_point = '1.71 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod7]
    type = LineValueSampler
    start_point = '1.72 4 0'
    end_point = '1.72 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod8]
    type = LineValueSampler
    start_point = '1.73 4 0'
    end_point = '1.73 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod9]
    type = LineValueSampler
    start_point = '1.74 4 0'
    end_point = '1.74 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod10]
    type = LineValueSampler
    start_point = '1.75 4 0'
    end_point = '1.75 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod11]
    type = LineValueSampler
    start_point = '1.76 4 0'
    end_point = '1.76 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod12]
    type = LineValueSampler
    start_point = '1.77 4 0'
    end_point = '1.77 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod13]
    type = LineValueSampler
    start_point = '1.78 4 0'
    end_point = '1.78 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod14]
    type = LineValueSampler
    start_point = '1.79 4 0'
    end_point = '1.79 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod15]
    type = LineValueSampler
    start_point = '1.8 4 0'
    end_point = '1.8 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod16]
    type = LineValueSampler
    start_point = '1.81 4 0'
    end_point = '1.81 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod17]
    type = LineValueSampler
    start_point = '1.82 4 0'
    end_point = '1.82 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod18]
    type = LineValueSampler
    start_point = '1.83 4 0'
    end_point = '1.83 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod19]
    type = LineValueSampler
    start_point = '1.84 4 0'
    end_point = '1.84 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod20]
    type = LineValueSampler
    start_point = '1.85 4 0'
    end_point = '1.85 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod21]
    type = LineValueSampler
    start_point = '1.86 4 0'
    end_point = '1.86 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod22]
    type = LineValueSampler
    start_point = '1.87 4 0'
    end_point = '1.87 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod23]
    type = LineValueSampler
    start_point = '1.88 4 0'
    end_point = '1.88 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod24]
    type = LineValueSampler
    start_point = '1.89 4 0'
    end_point = '1.89 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod25]
    type = LineValueSampler
    start_point = '1.9 4 0'
    end_point = '1.9 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod26]
    type = LineValueSampler
    start_point = '1.91 4 0'
    end_point = '1.91 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod27]
    type = LineValueSampler
    start_point = '1.92 4 0'
    end_point = '1.92 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod28]
    type = LineValueSampler
    start_point = '1.93 4 0'
    end_point = '1.93 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod29]
    type = LineValueSampler
    start_point = '1.94 4 0'
    end_point = '1.94 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod30]
    type = LineValueSampler
    start_point = '1.95 4 0'
    end_point = '1.95 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod31]
    type = LineValueSampler
    start_point = '1.96 4 0'
    end_point = '1.96 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod32]
    type = LineValueSampler
    start_point = '1.97 4 0'
    end_point = '1.97 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod33]
    type = LineValueSampler
    start_point = '1.98 4 0'
    end_point = '1.98 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod34]
    type = LineValueSampler
    start_point = '1.99 4 0'
    end_point = '1.99 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod35]
    type = LineValueSampler
    start_point = '2.0 4 0'
    end_point = '2.0 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod36]
    type = LineValueSampler
    start_point = '2.01 4 0'
    end_point = '2.01 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod37]
    type = LineValueSampler
    start_point = '2.02 4 0'
    end_point = '2.02 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod38]
    type = LineValueSampler
    start_point = '2.03 4 0'
    end_point = '2.03 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod39]
    type = LineValueSampler
    start_point = '2.04 4 0'
    end_point = '2.04 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod40]
    type = LineValueSampler
    start_point = '2.05 4 0'
    end_point = '2.05 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod41]
    type = LineValueSampler
    start_point = '2.06 4 0'
    end_point = '2.06 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod42]
    type = LineValueSampler
    start_point = '2.07 4 0'
    end_point = '2.07 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod43]
    type = LineValueSampler
    start_point = '2.08 4 0'
    end_point = '2.08 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod44]
    type = LineValueSampler
    start_point = '2.09 4 0'
    end_point = '2.09 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod45]
    type = LineValueSampler
    start_point = '2.1 4 0'
    end_point = '2.1 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod46]
    type = LineValueSampler
    start_point = '2.11 4 0'
    end_point = '2.11 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod47]
    type = LineValueSampler
    start_point = '2.12 4 0'
    end_point = '2.12 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod48]
    type = LineValueSampler
    start_point = '2.13 4 0'
    end_point = '2.13 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod49]
    type = LineValueSampler
    start_point = '2.14 4 0'
    end_point = '2.14 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod50]
    type = LineValueSampler
    start_point = '2.15 4 0'
    end_point = '2.15 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod51]
    type = LineValueSampler
    start_point = '2.16 4 0'
    end_point = '2.16 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod52]
    type = LineValueSampler
    start_point = '2.17 4 0'
    end_point = '2.17 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod53]
    type = LineValueSampler
    start_point = '2.18 4 0'
    end_point = '2.18 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod54]
    type = LineValueSampler
    start_point = '2.19 4 0'
    end_point = '2.19 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod55]
    type = LineValueSampler
    start_point = '2.20 4 0'
    end_point = '2.20 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod56]
    type = LineValueSampler
    start_point = '2.21 4 0'
    end_point = '2.21 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod57]
    type = LineValueSampler
    start_point = '2.22 4 0'
    end_point = '2.22 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod58]
    type = LineValueSampler
    start_point = '2.23 4 0'
    end_point = '2.23 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod59]
    type = LineValueSampler
    start_point = '2.24 4 0'
    end_point = '2.24 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod60]
    type = LineValueSampler
    start_point = '2.25 4 0'
    end_point = '2.25 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod61]
    type = LineValueSampler
    start_point = '2.26 4 0'
    end_point = '2.26 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod62]
    type = LineValueSampler
    start_point = '2.27 4 0'
    end_point = '2.27 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod63]
    type = LineValueSampler
    start_point = '2.28 4 0'
    end_point = '2.28 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod64]
    type = LineValueSampler
    start_point = '2.29 4 0'
    end_point = '2.29 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod65]
    type = LineValueSampler
    start_point = '2.3 4 0'
    end_point = '2.3 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod66]
    type = LineValueSampler
    start_point = '2.35 4 0'
    end_point = '2.35 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod67]
    type = LineValueSampler
    start_point = '2.4 4 0'
    end_point = '2.4 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod68]
    type = LineValueSampler
    start_point = '2.45 4 0'
    end_point = '2.45 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
  [cod69]
    type = LineValueSampler
    start_point = '2.5 4 0'
    end_point = '2.5 0 0'
    variable = 'cod'
    num_points = 10000
    sort_by = 'y'
  []
[]

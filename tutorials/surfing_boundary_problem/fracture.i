[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = ${nx}
    ny = ${ny}
    xmax = 30
    ymin = -5
    ymax = 5
  []
[]

[Adaptivity]
  marker = marker
  initial_marker = marker
  initial_steps = ${refine}
  stop_time = 0
  max_h_level = ${refine}
  [Markers]
    [marker]
      type = BoxMarker
      bottom_left = '0 -0.7 0'
      top_right = '30 0.7 0'
      outside = DO_NOTHING
      inside = REFINE
    []
  []
[]

[Variables]
  [d]
    [InitialCondition]
      type = FunctionIC
      function = 'if(y=0&x>=0&x<=5,1,0)'
    []
  []
[]

[AuxVariables]
  [bounds_dummy]
  []
  [disp_x]
  []
  [disp_y]
  []
  [strain_zz]
  []
  [psie_active]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[Bounds]
  [conditional]
    type = VariableOldValueBounds
    variable = bounds_dummy
    bounded_variable = d
    bound_type = lower
  []
  [upper]
    type = ConstantBounds
    variable = bounds_dummy
    bounded_variable = d
    bound_type = upper
    bound_value = 1
  []
[]

[Kernels]
  [diff]
    type = ADPFFDiffusion
    variable = d
    fracture_toughness = Gc
    regularization_length = l
    normalization_constant = c0
  []
  [source]
    type = ADPFFSource
    variable = d
    free_energy = psi
  []
  [nuc_force]
    type = ADCoefMatSource
    variable = d
    prop_names = 'ce'
    coefficient = 1.0
  []
[]

[Materials]
  [fracture_properties]
    type = ADGenericConstantMaterial
    prop_names = 'E K G lambda Gc l sigma_ts sigma_hs'
    prop_values = '${E} ${K} ${G} ${Lambda} ${Gc} ${l} ${sigma_ts} ${sigma_hs}'
  []
  [degradation]
    type = PowerDegradationFunction
    property_name = g
    expression = (1-d)^p*(1-eta)+eta
    phase_field = d
    parameter_names = 'p eta '
    parameter_values = '2 0'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    property_name = alpha
    expression = 'd'
    phase_field = d
  []
  [psi]
    type = ADDerivativeParsedMaterial
    property_name = psi
    expression = 'g*psie_active+(Gc*delta/c0/l)*alpha'
    coupled_variables = 'd psie_active'
    material_property_names = 'delta alpha(d) g(d) Gc c0 l'
    derivative_order = 1
  []
  [nucleation_micro_force]
    type = LDLNucleationMicroForce
    phase_field = d
    degradation_function = g
    regularization_length = l
    normalization_constant = c0
    fracture_toughness = Gc
    tensile_strength = sigma_ts
    hydrostatic_strength = sigma_hs
    delta = delta
    h_correction = true
    external_driving_force_name = ce
  []
  [strain]
    type = ADComputePlaneSmallStrain
    out_of_plane_strain = 'strain_zz'
    displacements = 'disp_x disp_y'
  []
  [elasticity]
    type = SmallDeformationIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    decomposition = NONE
  []
  [stress]
    type = ComputeSmallDeformationStress
    elasticity_model = elasticity
    output_properties = 'stress'
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'
  automatic_scaling = true

  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10
[]

[Outputs]
  print_linear_residuals = false
[]

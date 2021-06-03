E = 1.2e5
nu = 0.24
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'
thickness = 1

# alpha = 1.5e-5
# T_full = '${fparse 562.78+273.15}'
# T_partial = '${fparse 543.40+273.15}'
# T_shutdown = '${fparse 25+273.15}'
# p_full = 17
# p_partial = 10
# p_shutdown = 0
#
# T0 = ${T_partial}
# p = ${p_partial}

# Stress benchmarks
# full load:
# stress_rr = -16.94 (stress_zz)
# stress_tt = 18.61  (stress_xx)
# stress_zz = -2.34  (stress_yy)
# partial load:
# stress_rr = -9.99  (stress_zz)
# stress_tt = -2.99  (stress_xx)
# stress_zz = -15.32 (stress_yy)
# shutdown load:
# stress_rr = -0.05  (stress_zz)
# stress_tt = 30.68  (stress_xx)
# stress_zz = 30.61  (stress_yy)

# h0 = 0.2
# H = 1e-3
#
# l = 0.5
# Gc = 1
# psic = 1

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = gold/cylinder.msh
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [disp_z]
  []
  [strain_zz]
    block = oxide_lower_dimensional_block
  []
[]

[AuxVariables]
  [d]
  []
  [stress_xx]
    order = CONSTANT
    family = MONOMIAL
    block = oxide_lower_dimensional_block
  []
  [stress_yy]
    order = CONSTANT
    family = MONOMIAL
    block = oxide_lower_dimensional_block
  []
  [stress_zz]
    order = CONSTANT
    family = MONOMIAL
    block = oxide_lower_dimensional_block
  []
[]

[AuxKernels]
  [stress_xx]
    type = ADRankTwoAux
    variable = stress_xx
    rank_two_tensor = stress
    index_i = 0
    index_j = 0
  []
  [stress_yy]
    type = ADRankTwoAux
    variable = stress_xx
    rank_two_tensor = stress
    index_i = 1
    index_j = 1
  []
  [stress_zz]
    type = ADRankTwoAux
    variable = stress_xx
    rank_two_tensor = stress
    index_i = 2
    index_j = 2
  []
[]

[Kernels]
  # metal
  [solid_x_metal]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    use_displaced_mesh = true
    block = metal
  []
  [solid_y_metal]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    use_displaced_mesh = true
    block = metal
  []
  [solid_z_metal]
    type = ADStressDivergenceTensors
    variable = disp_z
    component = 2
    use_displaced_mesh = true
    block = metal
  []
  # oxide
  [solid_x_oxide]
    type = ADLowerDimensionalStressDivergenceTensors
    variable = disp_x
    component = 0
    thickness = thickness
    use_displaced_mesh = true
    block = oxide_lower_dimensional_block
  []
  [solid_y_oxide]
    type = ADLowerDimensionalStressDivergenceTensors
    variable = disp_y
    component = 1
    thickness = thickness
    use_displaced_mesh = true
    block = oxide_lower_dimensional_block
  []
  [solid_z_oxide]
    type = ADLowerDimensionalStressDivergenceTensors
    variable = disp_z
    component = 2
    thickness = thickness
    use_displaced_mesh = true
    block = oxide_lower_dimensional_block
  []
  [pressure]
    type = ADLowerDimensionalWeakOutOfPlaneStress
    variable = strain_zz
    out_of_plane_stress = 1e5*t
    block = oxide_lower_dimensional_block
  []
[]

[BCs]
  [fix_outer_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'outer'
    value = 0
  []
  [fix_outer_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'outer'
    value = 0
  []
  [fix_outer_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'outer'
    value = 0
  []
[]

[Materials]
  [oxide_thickness]
    type = GenericConstantMaterial
    prop_names = 'thickness'
    prop_values = '${thickness}'
  []
  [bulk_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G'
    prop_values = '${K} ${G}'
  []
  # fracture
  [degradation]
    type = NoDegradation
    f_name = g
    phase_field = d
  []
  # elasticity
  [deformation_gradient_metal]
    type = ComputeDeformationGradient
    block = metal
  []
  [deformation_gradient_oxide]
    type = ComputeLowerDimensionalDeformationGradient
    out_of_plane_strain = strain_zz
    block = oxide_lower_dimensional_block
  []
  [hencky]
    type = HenckyIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    decomposition = NONE
  []
  [stress]
    type = ComputeLargeDeformationStress
    elasticity_model = hencky
  []
[]

[VectorPostprocessors]
  [disp_x]
    type = LineValueSampler
    variable = disp_x
    start_point = '14 0 0'
    end_point = '25 0 0'
    num_points = 100
    sort_by = x
    execute_on = FINAL
  []
  [disp_y]
    type = LineValueSampler
    variable = disp_y
    start_point = '14 0 0'
    end_point = '25 0 0'
    num_points = 100
    sort_by = x
    execute_on = FINAL
  []
  [disp_z]
    type = LineValueSampler
    variable = disp_z
    start_point = '14 0 0'
    end_point = '25 0 0'
    num_points = 100
    sort_by = x
    execute_on = FINAL
  []
  [stress_rr]
    type = LineValueSampler
    variable = stress_xx
    start_point = '14 0 0'
    end_point = '14 0 20'
    num_points = 100
    sort_by = z
    execute_on = FINAL
  []
  [stress_tt]
    type = LineValueSampler
    variable = stress_yy
    start_point = '14 0 0'
    end_point = '14 0 20'
    num_points = 100
    sort_by = z
    execute_on = FINAL
  []
  [stress_zz]
    type = LineValueSampler
    variable = stress_zz
    start_point = '14 0 0'
    end_point = '14 0 20'
    num_points = 100
    sort_by = z
    execute_on = FINAL
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'

  dt = 0.1
  end_time = 1

  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08

  automatic_scaling = true
  abort_on_solve_fail = true
[]

[Outputs]
  print_linear_residuals = false
  exodus = true
  [csv]
    type = CSV
    execute_vector_postprocessors_on = FINAL
  []
[]

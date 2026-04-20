# Restart/recovery leg of the TET10 recover/remesh unit test.
#
# Reads the Exodus output produced by reference.i through a SolutionUserObjectQP
# on the SAME generated TET10 mesh.  11 QPs/elem (TET10_4th), FOURTH-order
# quadrature.  Centroid-to-centroid lookup is exact on the same mesh, so the QP
# fields (be_bar, stress, rotation_tensor, stretch_tensor, effective_plastic_strain,
# ...) should be copied byte-for-byte.

E = 201.8e3
nu = 0.3
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'
rho = 7900
Gc = 6
l = 0.2
psic = 15
Q = 0.9
specific_heat = 4.47e-4
thermal_conductivity = 4.4e-4
c1 = 0.1
c2 = 0.9
c3 = 0.1

hht_alpha = -0.25
newmark_beta = '${fparse (1-hht_alpha)^2/4}'
newmark_gamma = '${fparse 1/2-hht_alpha}'

# Must match reference.i
ref_end_time = 1
dt = 0.1
start_time = '${ref_end_time}'
end_time = 1.5

trans_time = 1.0
final_velocity = 0.05

recover_file = reference_out_disp.e

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  volumetric_locking_correction = true
  element = TET10_4th
[]

[Problem]
  type = ReferenceResidualProblem
  extra_tag_vectors = 'ref'
  reference_vector = 'ref'
[]

[Mesh]
  # The restart consumes the exact mesh (and state) produced by reference.i.
  # Using FileMeshGenerator on reference_out.e guarantees identical node/element
  # numbering, so SolutionUserObjectQP's centroid-to-centroid lookup is exact.
  [fmg]
    type = FileMeshGenerator
    file = ${recover_file}
  []
[]

[UserObjects]
  [epsol]
    type = SolutionUserObjectQP
    mesh = ${recover_file}
    system_variables = 'd d_old d_corr psie_corr_active psip_active T accel_x accel_y accel_z vel_x vel_y vel_z'
    materials = 'effective_plastic_strain'
    tensor_materials = 'stress be_bar stretch_tensor rotation_tensor'
    nodal_variable_order = SECOND
    use_displaced_mesh = false
    execute_on = 'INITIAL'
    timestep = LATEST
  []
[]

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'fracture_restart.i'
    cli_args = 'Gc=${Gc};l=${l};start_time=${start_time};recover_file=${recover_file};out_file=restart_d'
    execute_on = 'TIMESTEP_END'
    clone_parent_mesh = no
  []
[]

[Transfers]
  [to_coal]
    type = MultiAppCopyTransfer
    variable = coal
    source_variable = coal
    to_multi_app = fracture
  []
  [from_d]
    type = MultiAppCopyTransfer
    variable = d
    source_variable = d
    from_multi_app = fracture
  []
  [to_psie]
    type = MultiAppCopyTransfer
    variable = psie_corr_active
    source_variable = psie_corr_active
    to_multi_app = fracture
  []
  [to_psip]
    type = MultiAppCopyTransfer
    variable = psip_active
    source_variable = psip_active
    to_multi_app = fracture
  []
[]

[Variables]
  [disp_x]
    order = SECOND
  []
  [disp_y]
    order = SECOND
  []
  [disp_z]
    order = SECOND
  []
  [T]
    order = SECOND
    [InitialCondition]
      type = SolutionIC
      from_variable = T
      variable = T
      solution_uo = epsol
    []
  []
[]

[AuxVariables]
  [d]
    order = SECOND
    [InitialCondition]
      type = SolutionIC
      from_variable = d
      variable = d
      solution_uo = epsol
    []
  []
  # Irreversibility machinery (same as reference.i): d_old carries the
  # previous-step d, d_corr = min(1, max(d_old, max(0, d))) is the clipped,
  # monotone damage that actually feeds every damage-dependent material.  Both
  # are seeded from the reference's last-step values.
  [d_old]
    order = SECOND
    [InitialCondition]
      type = SolutionIC
      from_variable = d_old
      variable = d_old
      solution_uo = epsol
    []
  []
  [d_corr]
    order = SECOND
    [InitialCondition]
      type = SolutionIC
      from_variable = d_corr
      variable = d_corr
      solution_uo = epsol
    []
  []
  [psie_corr_active]
    order = FIRST
    family = MONOMIAL
    [InitialCondition]
      type = SolutionIC
      from_variable = psie_corr_active
      variable = psie_corr_active
      solution_uo = epsol
    []
  []
  [psip_active]
    order = FIRST
    family = MONOMIAL
    [InitialCondition]
      type = SolutionIC
      from_variable = psip_active
      variable = psip_active
      solution_uo = epsol
    []
  []

  [accel_x]
    order = SECOND
    [InitialCondition]
      type = SolutionIC
      from_variable = accel_x
      variable = accel_x
      solution_uo = epsol
    []
  []
  [vel_x]
    order = SECOND
    [InitialCondition]
      type = SolutionIC
      from_variable = vel_x
      variable = vel_x
      solution_uo = epsol
    []
  []
  [accel_y]
    order = SECOND
    [InitialCondition]
      type = SolutionIC
      from_variable = accel_y
      variable = accel_y
      solution_uo = epsol
    []
  []
  [vel_y]
    order = SECOND
    [InitialCondition]
      type = SolutionIC
      from_variable = vel_y
      variable = vel_y
      solution_uo = epsol
    []
  []
  [accel_z]
    order = SECOND
    [InitialCondition]
      type = SolutionIC
      from_variable = accel_z
      variable = accel_z
      solution_uo = epsol
    []
  []
  [vel_z]
    order = SECOND
    [InitialCondition]
      type = SolutionIC
      from_variable = vel_z
      variable = vel_z
      solution_uo = epsol
    []
  []
[]

[AuxKernels]
  [psie_corr_active]
    type = ADMaterialRealAux
    variable = psie_corr_active
    property = psie_corr_active
    execute_on = 'TIMESTEP_END'
  []
  [psip_active]
    type = ADMaterialRealAux
    variable = psip_active
    property = psip_active
    execute_on = 'TIMESTEP_END'
  []
  [d_old]
    type = CopyValueAux
    source = d
    variable = d_old
    execute_on = 'TIMESTEP_END'
  []
  [d_corr]
    type = ParsedAux
    variable = d_corr
    coupled_variables = 'd d_old'
    expression = 'min(1,max(d_old,max(0,d)))'
  []

  [accel_x]
    type = NewmarkAccelAux
    variable = accel_x
    displacement = disp_x
    velocity = vel_x
    beta = ${newmark_beta}
    execute_on = 'timestep_end'
  []
  [vel_x]
    type = NewmarkVelAux
    variable = vel_x
    acceleration = accel_x
    gamma = ${newmark_gamma}
    execute_on = 'timestep_end'
  []
  [accel_y]
    type = NewmarkAccelAux
    variable = accel_y
    displacement = disp_y
    velocity = vel_y
    beta = ${newmark_beta}
    execute_on = 'timestep_end'
  []
  [vel_y]
    type = NewmarkVelAux
    variable = vel_y
    acceleration = accel_y
    gamma = ${newmark_gamma}
    execute_on = 'timestep_end'
  []
  [accel_z]
    type = NewmarkAccelAux
    variable = accel_z
    displacement = disp_z
    velocity = vel_z
    beta = ${newmark_beta}
    execute_on = 'timestep_end'
  []
  [vel_z]
    type = NewmarkVelAux
    variable = vel_z
    acceleration = accel_z
    gamma = ${newmark_gamma}
    execute_on = 'timestep_end'
  []
[]

[Kernels]
  [inertia_x]
    type = ADInertialForce
    variable = disp_x
    density = adj_density
    use_displaced_mesh = false
    beta = ${newmark_beta}
    gamma = ${newmark_gamma}
    alpha = ${hht_alpha}
    velocity = vel_x
    acceleration = accel_x
    absolute_value_vector_tags = 'ref'
  []
  [inertia_y]
    type = ADInertialForce
    variable = disp_y
    density = adj_density
    use_displaced_mesh = false
    beta = ${newmark_beta}
    gamma = ${newmark_gamma}
    alpha = ${hht_alpha}
    velocity = vel_y
    acceleration = accel_y
    absolute_value_vector_tags = 'ref'
  []
  [inertia_z]
    type = ADInertialForce
    variable = disp_z
    density = adj_density
    use_displaced_mesh = false
    beta = ${newmark_beta}
    gamma = ${newmark_gamma}
    alpha = ${hht_alpha}
    velocity = vel_z
    acceleration = accel_z
    absolute_value_vector_tags = 'ref'
  []
  [x]
    type = ADDynamicStressDivergenceTensorsRecover
    variable = disp_x
    component = 0
    use_displaced_mesh = true
    alpha = ${hht_alpha}
    solution = epsol
    absolute_value_vector_tags = 'ref'
  []
  [y]
    type = ADDynamicStressDivergenceTensorsRecover
    variable = disp_y
    component = 1
    use_displaced_mesh = true
    alpha = ${hht_alpha}
    solution = epsol
    absolute_value_vector_tags = 'ref'
  []
  [z]
    type = ADDynamicStressDivergenceTensorsRecover
    variable = disp_z
    component = 2
    use_displaced_mesh = true
    alpha = ${hht_alpha}
    solution = epsol
    absolute_value_vector_tags = 'ref'
  []
  [hcond_time]
    type = ADHeatConductionTimeDerivative
    variable = T
    density_name = density
    specific_heat = specific_heat
    absolute_value_vector_tags = 'ref'
  []
  [hcond]
    type = ADHeatConduction
    variable = T
    thermal_conductivity = thermal_conductivity
    absolute_value_vector_tags = 'ref'
  []
  [heat_source]
    type = ADCoefMatSource
    variable = T
    coefficient = -1
    prop_names = 'plastic_heat_generation'
    absolute_value_vector_tags = 'ref'
  []
[]

[Functions]
  [ypull_func]
    type = ParsedFunction
    expression = 'if(t<=trans,v/(2*trans)*t*t,v*t-v*trans/2)'
    symbol_names = 'trans v'
    symbol_values = '${trans_time} ${final_velocity}'
  []
[]

[Materials]
  [stress_sol]
    type = SolutionTensor
    solution = epsol
    tensor_name = stress
  []
  [stress_old_sol]
    type = ADGenericConstantRankTwoTensor
    tensor_name = stress_old_store_sol
    tensor_values = '1 0 0 0 1 0 0 0 1'
  []
  [coalescence]
    type = ADParsedMaterial
    property_name = coal
    material_property_names = 'effective_plastic_strain'
    constant_names = 'c1 c2 c3'
    constant_expressions = '${c1} ${c2} ${c3}'
    expression = ((1-c1)/(1+exp((effective_plastic_strain-c2)/c3))+c1)
    outputs = exodus
    output_properties = 'coal'
  []
  [defgrad]
    type = ComputeDeformationGradient
    recover = true
    solution = epsol
    recover_mode = polar_decomposition
  []
  [bulk_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G l Gc psic density thermal_conductivity specific_heat'
    prop_values = '${K} ${G} ${l} ${Gc} ${psic} ${rho} ${thermal_conductivity} ${specific_heat}'
  []
  [dens]
    type = ADStrainAdjustedDensityCustom
    strain_free_density = density
    base_name = 'adj'
  []
  [nodeg]
    type = NoDegradation
    phase_field = d_corr
    property_name = nodeg
  []
  [g]
    type = PowerDegradationFunction
    phase_field = d_corr
    property_name = g
    expression = (1-d_corr)^p*(1-eta)+eta
    parameter_names = 'p eta'
    parameter_values = '2 1e-6'
  []
  [hencky]
    type = CNHIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d_corr
    degradation_function = g
    decomposition = NONE
  []
  [J2]
    type = LargeDeformationJ2PlasticityCorrection
    phase_field = d_corr
    hardening_model = JC
    relative_tolerance = 1e-08
    recover = true
    solution = epsol
    output_properties = 'effective_plastic_strain'
    outputs = exodus
  []
  [JC]
    type = JohnsonCookHardening
    T = T
    taylor_quinney_factor = ${Q}
    sigma_0 = 1
    T0 = 280
    reference_plastic_strain = 1
    reference_plastic_strain_rate = 1e-6
    phase_field = d_corr
    degradation_function = g
    A = 791.2
    B = 509.51
    C = 0.014
    n = 0.26
    m = 1.03
    Tm = 1033
    output_properties = 'plastic_heat_generation'
    outputs = exodus
  []
  [stress]
    type = ComputeLargeDeformationStress
    elasticity_model = hencky
    plasticity_model = J2
  []
[]

[BCs]
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = 'left top'
    value = 0
    preset = false
  []
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
    preset = false
  []
  [zfix]
    type = DirichletBC
    variable = disp_z
    boundary = 'front back'
    value = 0
    preset = false
  []
  [ypull]
    type = PresetDisplacement
    variable = disp_y
    boundary = top
    function = ypull_func
    beta = ${newmark_beta}
    velocity = vel_y
    acceleration = accel_y
  []
[]

[Postprocessors]
  [psie_corr_active_int]
    type = ADElementIntegralMaterialProperty
    mat_prop = psie_corr_active
    use_displaced_mesh = true
  []
  [psip_active_int]
    type = ADElementIntegralMaterialProperty
    mat_prop = psip_active
    use_displaced_mesh = true
  []
  [ep_int]
    type = ADElementIntegralMaterialProperty
    mat_prop = effective_plastic_strain
    use_displaced_mesh = true
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  line_search = none
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre    boomeramg'

  [TimeIntegrator]
    type = ImplicitEuler
  []

  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10
  nl_max_its = 100

  [TimeStepper]
    type = ConstantDT
    dt = ${dt}
  []
  [Quadrature]
    order = FOURTH
  []

  start_time = ${start_time}
  end_time = ${end_time}

  automatic_scaling = true

  fixed_point_max_its = 25
  fixed_point_rel_tol = 1e-7
  fixed_point_abs_tol = 1e-8
  accept_on_max_fixed_point_iteration = true
  abort_on_solve_fail = true
[]

[Outputs]
  print_linear_residuals = false
  [exodus]
    type = Exodus
    file_base = restart_out
    use_displaced = false
    execute_on = 'TIMESTEP_END'
  []
  [csv]
    type = CSV
    file_base = restart_out
    execute_on = 'TIMESTEP_END'
  []
[]

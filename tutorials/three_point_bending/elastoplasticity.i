mesh_file = 'gold/beam.msh'

psic = 90
Gc = 20
l = 0.027

E = 7.25e4
nu = 0.34
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'
eta = 1

sigma_y = 345
n = 1
ep0 = 0.345

[GlobalParams]
  displacements = 'disp_x disp_y'
  volumetric_locking_correction = true
[]

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    cli_args = 'mesh_file=${mesh_file};Gc=${Gc};psic=${psic};l=${l}'
  []
[]

[Transfers]
  [from_d]
    type = MultiAppCopyTransfer
    from_multi_app = 'fracture'
    source_variable = d
    variable = d
  []
  [to_psie_active]
    type = MultiAppCopyTransfer
    to_multi_app = 'fracture'
    source_variable = psie_active
    variable = psie_active
  []
  [to_psip_active]
    type = MultiAppCopyTransfer
    to_multi_app = 'fracture'
    source_variable = psip_active
    variable = psip_active
  []
[]

[Problem]
  kernel_coverage_check = false
  material_coverage_check = false
  boundary_restricted_node_integrity_check = false
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = ${mesh_file}
  []
  [active]
    type = SubdomainBoundingBoxGenerator
    input = fmg
    block_id = 0
    bottom_left = '0 0 0'
    top_right = '100 16 0'
  []
  [inactive]
    type = SubdomainBoundingBoxGenerator
    input = active
    block_id = 1
    bottom_left = '100 0 0'
    top_right = '200 16 0'
  []
[]

[UserObjects]
  [esm]
    type = CoupledVarThresholdElementSubdomainModifier
    coupled_var = d
    threshold = 0.925
    criterion_type = ABOVE
    subdomain_id = 1
    complement_subdomain_id = 0
    execute_on = 'INITIAL TIMESTEP_END'
    apply_initial_conditions = false
  []
[]

[Variables]
  [disp_x]
    block = 0
  []
  [disp_y]
    block = 0
  []
[]

[AuxVariables]
  [d]
  []
  [fy]
  []
[]

[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    component = 0
    use_displaced_mesh = true
    block = 0
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
    use_displaced_mesh = true
    save_in = fy
    block = 0
  []
[]

[Materials]
  [fracture_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G l Gc psic viscosity'
    prop_values = '${K} ${G} ${l} ${Gc} ${psic} ${eta}'
    block = 0
  []
  [degradation]
    type = ADDerivativeParsedMaterial
    property_name = g
    coupled_variables = d
    expression = '(1-d)^2/(1+(0.5*Gc/c0/l/psic-1)*d)^2*(1-eta)+eta'
    material_property_names = 'Gc c0 l psic'
    constant_names = 'eta '
    constant_expressions = '5e-3'
    derivative_order = 1
    block = 0
  []
  [crack_geometric]
    type = CrackGeometricFunction
    property_name = alpha
    expression = 'd'
    phase_field = d
    block = 0
  []
  [defgrad]
    type = ComputeDeformationGradient
    block = 0
  []
  [hencky]
    type = HenckyIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    output_properties = 'psie_active'
    outputs = exodus
    block = 0
  []
  [power_law_hardening]
    type = PowerLawHardening
    yield_stress = ${sigma_y}
    exponent = ${n}
    reference_plastic_strain = ${ep0}
    phase_field = d
    degradation_function = g
    output_properties = 'psip_active'
    outputs = exodus
    block = 0
  []
  [J2]
    type = LargeDeformationJ2Plasticity
    hardening_model = power_law_hardening
    output_properties = 'effective_plastic_strain'
    outputs = exodus
    block = 0
  []
  [newtonian_viscosity]
    type = LargeDeformationNewtonianViscosity
    block = 0
  []
  [stress]
    type = ComputeLargeDeformationStress
    elasticity_model = hencky
    plasticity_model = J2
    viscoelasticity_model = newtonian_viscosity
    block = 0
  []
[]

[BCs]
  [ydisp]
    type = FunctionDirichletBC
    variable = 'disp_y'
    boundary = 'left right'
    function = 't'
    preset = false
  []
  [top_xfix]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'top'
    value = 0
  []
  [top_yfix]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'top'
    value = 0
  []
[]

[Postprocessors]
  [force]
    type = NodalSum
    variable = fy
    boundary = top
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist                 '

  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-06
  nl_max_its = 50

  automatic_scaling = true
  [TimeStepper]
    type = FunctionDT
    function = '0.05'
    growth_factor = 1.5
    cutback_factor_at_failure = 0.5
  []
  end_time = 6.4
[]

[Outputs]
  print_linear_residuals = false
  csv = true
  exodus = true
[]

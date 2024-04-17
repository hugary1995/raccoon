E = 3e4
nu = 0.2
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

p = 0.5

Gc = 0.12
sigmac = 3
psic = '${fparse sigmac^2/2/E}'
l = 20
R = 10
h = '${fparse l/R}'
nx = '${fparse ceil(200/h)}'

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = fracture.i
    cli_args = 'p=${p};Gc=${Gc};psic=${psic};l=${l};nx=${nx}'
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [from_fracture]
    type = MultiAppCopyTransfer
    from_multi_app = 'fracture'
    variable = d
    source_variable = d
  []
  [to_fracture]
    type = MultiAppCopyTransfer
    to_multi_app = 'fracture'
    variable = 'psie_active disp_x disp_y'
    source_variable = 'psie_active disp_x disp_y'
  []
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    xmax = 200
    ymax = 1
    nx = ${nx}
    ny = 1
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
[]

[AuxVariables]
  [fx]
  []
  [d]
  []
[]

[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    save_in = fx
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
  []
  [pressure_x]
    type = ADPressurizedCrack
    variable = disp_x
    pressure = p
    phase_field = d
    indicator_function = I
    component = 0
  []
  [pressure_y]
    type = ADPressurizedCrack
    variable = disp_y
    pressure = p
    phase_field = d
    indicator_function = I
    component = 1
  []
[]

[BCs]
  [xdisp]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = right
    function = 't'
  []
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
  []
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  []
[]

[Materials]
  [bulk_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G l Gc psic p'
    prop_values = '${K} ${G} ${l} ${Gc} ${psic} ${p}'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    property_name = alpha
    expression = '2*d-d^2'
    phase_field = d
  []
  [indicator]
    type = ADDerivativeParsedMaterial
    property_name = I
    coupled_variables = 'd'
    expression = 'd^2'
    derivative_order = 1
  []
  [degradation]
    type = RationalDegradationFunction
    property_name = g
    expression = (1-d)^p/((1-d)^p+(Gc/psic*xi/c0/l)*d*(1+a2*d+a2*a3*d^2))*(1-eta)+eta
    phase_field = d
    material_property_names = 'Gc psic xi c0 l '
    parameter_names = 'p a2 a3 eta '
    parameter_values = '2 -0.5 0 1e-6'
  []
  [strain]
    type = ADComputeSmallStrain
  []
  [elasticity]
    type = SmallDeformationIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    decomposition = NONE
    output_properties = 'psie_active'
    outputs = exodus
  []
  [stress]
    type = ComputeSmallDeformationStress
    elasticity_model = elasticity
  []
  [pressure_density]
    type = PressureDensity
    effective_pressure_density = p_density
    phase_field = d
    pressure = p
    indicator_function = I
  []
[]

[Postprocessors]
  [Fx_left]
    type = NodalSum
    variable = fx
    boundary = left
  []
  [Fx_right]
    type = NodalSum
    variable = fx
    boundary = right
  []
  [effective_pressure]
    type = ADElementIntegralMaterialProperty
    mat_prop = p_density
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist                 '
  automatic_scaling = true

  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10

  dt = 5e-4
  end_time = 5e-2

  fixed_point_max_its = 200
  accept_on_max_fixed_point_iteration = false
  fixed_point_rel_tol = 1e-8
  fixed_point_abs_tol = 1e-10
[]

[Outputs]
  file_base = 'p_${p}_l_${l}'
  exodus = true
  csv = true
  print_linear_residuals = false
[]

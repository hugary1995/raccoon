a = 250
ratio = 4

sigma_cs = 20000
sigma_ts = 2500
gamma_1 = '${fparse (sigma_cs-sigma_ts)/sqrt(3)/(sigma_cs+sigma_ts)}'
gamma_0 = '${fparse 2*sigma_cs*sigma_ts/sqrt(3)/(sigma_cs+sigma_ts)}'
psic = 15
Gc = 1.38e5
l = '${fparse ratio * a}'

E = 2.1e5
nu = 0.3
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

eta = 1e-3

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  volumetric_locking_correction = false
[]

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    cli_args = 'a=${a};psic=${psic};Gc=${Gc};l=${l};eta=${eta}'
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [from_d]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    variable = d
    source_variable = d
  []
  [to_psin_active]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    variable = psin_active
    source_variable = psin_active
  []
[]

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 3
    xmax = ${a}
    ymax = ${a}
    zmax = ${a}
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [disp_z]
  []
[]

[AuxVariables]
  [d]
  []
  [stress]
    order = CONSTANT
    family = MONOMIAL
  []
  [total_strain]
    order = CONSTANT
    family = MONOMIAL
  []
  [elastic_strain]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [stress]
    type = ADRankTwoAux
    variable = stress
    rank_two_tensor = stress
    index_i = 1
    index_j = 1
  []
  [total_strain]
    type = ADRankTwoAux
    variable = total_strain
    rank_two_tensor = total_strain
    index_i = 1
    index_j = 1
  []
  [elastic_strain]
    type = ADRankTwoAux
    variable = elastic_strain
    rank_two_tensor = elastic_strain
    index_i = 1
    index_j = 1
  []
[]

[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
  []
  [solid_z]
    type = ADStressDivergenceTensors
    variable = disp_z
    component = 2
  []
[]

[Materials]
  [bulk_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G l Gc psic'
    prop_values = '${K} ${G} ${l} ${Gc} ${psic}'
  []
  # [degradation]
  #   type = RationalDegradationFunction
  #   f_name = g
  #   function = (1-d)^p/((1-d)^p+(Gc/psic*xi/c0/l)*d*(1+a2*d+a2*a3*d^2))*(1-eta)+eta
  #   phase_field = d
  #   material_property_names = 'Gc psic xi c0 l '
  #   parameter_names = 'p a2 a3 eta '
  #   parameter_values = '2 1 0 ${eta}'
  # []
  [degradation]
    type = PowerDegradationFunction
    f_name = g
    function = (1-d)^p*(1-eta)+eta
    phase_field = d
    parameter_names = 'p eta '
    parameter_values = '2 ${eta}'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    phase_field = d
  []
  [strain]
    type = ADComputeSmallStrain
  []
  [stress]
    type = ComputeStressWithStrengthSurface
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    gamma_1 = ${gamma_1}
    gamma_0 = ${gamma_0}
    structure_energy_density = psin
    outputs = exodus
    output_properties = 'psin_active'
  []
[]

[BCs]
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = 'left'
    value = 0
  []
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  []
  [zfix]
    type = DirichletBC
    variable = disp_z
    boundary = 'back'
    value = 0
  []
  [ydisp]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'top'
    function = 't'
    preset = false
  []
[]

[Postprocessors]
  [total_strain]
    type = ElementAverageValue
    variable = total_strain
  []
  [elastic_strain]
    type = ElementAverageValue
    variable = elastic_strain
  []
  [stress]
    type = ElementAverageValue
    variable = stress
  []
  [d]
    type = ElementAverageValue
    variable = d
  []
  [psin_active]
    type = ADElementIntegralMaterialProperty
    mat_prop = psin_active
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  nl_rel_tol = 1e-08
  nl_abs_tol = 1e-10
  nl_max_its = 50

  dt = '${fparse 0.0005 * a}'
  end_time = '${fparse 0.5 * a}'

  automatic_scaling = true

  picard_max_its = 100
  picard_rel_tol = 1e-08
  picard_abs_tol = 1e-10
  accept_on_max_picard_iteration = true
  abort_on_solve_fail = true
[]

[Outputs]
  file_base = 'stress_strain'
  print_linear_residuals = false
  csv = true
  exodus = true
[]

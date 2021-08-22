E = 1
nu = 0.3
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

Gc = 1
l = 0.02

N = 200

a = 0.2

[MultiApps]
  [fracture]
    type = FullSolveMultiApp
    input_files = fracture.i
    cli_args = 'Gc=${Gc};l=${l};N=${N};a=${a}'
    execute_on = 'INITIAL'
  []
  [levelset]
    type = FullSolveMultiApp
    input_files = levelset.i
    cli_args = 'N=${N};a=${a}'
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
  [to_levelset]
    type = MultiAppCopyTransfer
    multi_app = levelset
    direction = to_multiapp
    variable = 'd disp_x disp_y'
    source_variable = 'd disp_x disp_y'
  []
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = ${N}
    ny = ${N}
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
[]

[AuxVariables]
  [d]
  []
  [phi]
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
[]

[BCs]
  [ydisp]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'top'
    function = 't'
  []
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  []
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = 'top bottom'
    value = 0
  []
[]

[Materials]
  [bulk_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G'
    prop_values = '${K} ${G}'
  []
  [degradation]
    type = PowerDegradationFunction
    f_name = g
    function = (1-d)^p*(1-eta)+eta
    phase_field = d
    parameter_names = 'p eta '
    parameter_values = '2 1e-8'
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
  []
  [stress]
    type = ComputeSmallDeformationStress
    elasticity_model = elasticity
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

  dt = 0.01
  num_steps = 1
[]

[Outputs]
  print_linear_residuals = false
[]

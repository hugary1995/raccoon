E = 4000
nu = 0.2
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'
lambda = '${fparse K-2*G/3}'

Gc = 1
psic = 2.64
l = 0.02
k = 1e-6

v = '${fparse sqrt(Gc*3/lambda)}'

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    cli_args = 'Gc=${Gc};l=${l};psic=${psic};k=${k}'
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [from_d]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = d
    variable = d
  []
  [to_psie_active]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    variable = psie_active
    source_variable = psie_active
  []
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = 'gold/matrix.msh'
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
  [forcing]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'top'
    function = '${v}*t'
    preset = false
  []
  [fixed_hole_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'hole'
    value = 0
  []
  [fixed_hole_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'hole'
    value = 0
  []
[]

[Materials]
  [bulk_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G l Gc psic'
    prop_values = '${K} ${G} ${l} ${Gc} ${psic}'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = d
    phase_field = d
  []
  [degradation]
    type = RationalDegradationFunction
    f_name = g
    phase_field = d
    parameter_names = 'p a2 a3 eta '
    parameter_values = '2 1 0 ${k}'
    material_property_names = 'Gc psic xi c0 l '
  []
  [elasticity]
    type = SmallDeformationIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    decomposition = VOLDEV
    output_properties = 'psie_active'
    outputs = exodus
  []
  [strain]
    type = ADComputeSmallStrain
  []
  [stress]
    type = ComputeSmallDeformationStress
    elasticity_model = elasticity
    output_properties = 'stress'
    outputs = exodus
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'
  dt = 0.01
  end_time = 3
  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06
  automatic_scaling = true
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

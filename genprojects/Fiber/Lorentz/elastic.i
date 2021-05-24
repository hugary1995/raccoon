E = 4000
nu = 0.2
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'
lambda = '${fparse K-2*G/3}'
Gc = 1e-3
l = 0.1
k = 2e-4
psic = 0.0002636
#psic = 1

v = '${fparse -sqrt(Gc*3/lambda)}'

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    cli_args = 'Gc=${Gc};l=${l};k=${k};psic=${psic}'
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
    file = '../gold/domain05Coarse.msh'
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [strain_zz]
  []
[]

[AuxVariables]
  [d]
  []
  [stress_xx]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_yy]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [stress_xx]
    type = ADRankTwoAux
    variable = 'stress_xx'
    rank_two_tensor = 'stress'
    index_i = 0
    index_j = 0
    execute_on = 'TIMESTEP_END'
  []
  [stress_yy]
    type = ADRankTwoAux
    variable = 'stress_yy'
    rank_two_tensor = 'stress'
    index_i = 0
    index_j = 0
    execute_on = 'TIMESTEP_END'
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
  [plane_stress]
    type = ADWeakPlaneStress
    variable = strain_zz
  []
[]


[BCs]
  [forcing]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'Top'
    function = '${v}*t'
    preset = false
  []
  [FixedHole_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'Hole'
    value = 0
  []
  [FixedHole_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'Hole'
    value = 0
    []
[]

[Materials]

  [elasticity]
    type = SmallDeformationIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    decomposition = VOLDEV
    output_properties = 'elastic_strain psie_active'
    outputs = exodus
  []
  [strain]
    type = ADComputePlaneSmallStrain
    out_of_plane_strain = strain_zz
    displacements = 'disp_x disp_y'
  []
  [degradation]
    type = RationalDegradationFunction
    f_name = g
    phase_field = d
    parameter_names = 'p a2 a3 eta'
    parameter_values = '2 1 0 1e-04'
  []
  [stress]
    type = ComputeSmallDeformationStress
    elasticity_model = elasticity
    output_properties = 'stress'
    outputs = exodus
  []
  [bulk_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G l Gc psic'
    prop_values = '${K} ${G} ${l} ${Gc} ${psic}'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    phase_field = d
  []

[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
#  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  #petsc_options_value = 'lu       superlu_dist'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type'
  petsc_options_value = 'lu      ilu          200         200                0                     vinewtonrsls'
  #dt = 0.00492
  dt = 0.01
  end_time =8
  nl_abs_tol = 1e-06
  nl_rel_tol = 1e-06
  automatic_scaling = true
  compute_scaling_once = false
  fp_max_its = 0
  fp_tol = 1e-06
  accept_on_max_fp_iteration = true
#  picard_max_its = 100
  #picard_abs_tol = 1e-50
  #picard_rel_tol = 1e-03
  #accept_on_max_picard_iteration = false
[]

[Outputs]
  file_base = 'comp'
  exodus = true
  interval = 1
[]

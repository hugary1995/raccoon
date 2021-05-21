E = 4000
nu = 0.2
Gc = 100
l = 0.02
#psic = 0.4
k = 1e-06


[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'fractureQD.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
    cli_args = 'Gc=${Gc};l=${l};k=${k}'
  []
[]

[Transfers]
  [from_d]
    type = MultiAppCopyTransfer
    multi_app = 'fracture'
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd'
  []
  [to_E_el_active]
    type = MultiAppCopyTransfer
    multi_app = 'fracture'
    direction = to_multiapp
    source_variable = 'E_el_active'
    variable = 'E_el_active'
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
  [E_el_active]
    order = CONSTANT
    family = MONOMIAL
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

  [E_el]
    type = ADMaterialRealAux
    variable = 'E_el_active'
    property = 'E_el_active'
    execute_on = 'TIMESTEP_END'
  []
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
  [plane_stress]
    type = ADWeakPlaneStress
    variable = 'strain_zz'
    displacements = 'disp_x disp_y'
  []
[]


[BCs]
  [forcing]
    type = FunctionDirichletBC
    variable = 'disp_y'
    boundary = 'Top'
    #//function = '-t/50'
    #function = 'if(t<4.0, 0.0175*t, 0.005*t )'
    function = '0.1*t'
    #preset = false
    #use_displaced_mesh = true
  []

  [FixedHole_x]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'Hole'
    value = 0
    #use_displaced_mesh = true
  []
  [FixedHole_y]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'Hole'
    value = 0
    #use_displaced_mesh = true
  []
[]

[Materials]

  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = ${E}
    poissons_ratio = ${nu}
  []
  #[strain]
  #  type = ADComputeSmallStrain
  #  displacements = 'disp_x disp_y'
  #]
  [strain]
    type = ADComputePlaneSmallStrain
    out_of_plane_strain = 'strain_zz'
    displacements = 'disp_x disp_y'
  []
  [stress]
    type = SmallStrainDegradedElasticPK2Stress_StrainVolDev
    d = 'd'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd^2'
    phase_field = d
  []
  [fracture_properties]
    type = ADGenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length'
    prop_values = '${Gc} ${l}' #${psic}'

  []
  [local_dissipation]
    #type = LinearLocalDissipation
    type = QuadraticLocalDissipation
    d = d
  []
  [phase_field_properties]
    type = ADFractureMaterial
    #local_dissipation_norm = 8/3
    local_dissipation_norm = 2
  []
  [degradation]
    type = QuadraticDegradation
    #type = LorentzDegradation
    d = d
    residual_degradation = ${k}
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
  end_time =20
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
  file_base = 'Elastic_squarewvoidsmallstep_quad_Traction_Brittle_VolDev_Res_BC_QD_NoIrr'
  exodus = true
  interval = 1
[]

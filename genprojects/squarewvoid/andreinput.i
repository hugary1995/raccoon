E = 2.7e5
nu = 0.25
Gc = 270
l = 0.1
psic = 0
k = 1e-4
dc = 2
[Problem]
  type = FixedPointProblem
[]
[Mesh]
   construct_side_list_from_node_list = true
   parallel_type = distributed
  [./mesh]
   type = FileMeshGenerator
   file = 'gold3/geo.msh'
  [../]
  [./all_nodes]
   type = BoundingBoxNodeSetGenerator
   input = mesh
   new_boundary = all_nodes
   bottom_left = '-1.51 -1.51 -0.01'
   top_right = '1.51 1.51 1.01'
  [../]
[]
[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
  [./d]
  [../]
[]
# [AuxVariables]
#   [./bounds_dummy]
#   [../]
# []
[UserObjects]
  [./E_driving]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'E_el_active'
  [../]
[]
# [Bounds]
#   [./irreversibility]
#     type = VariableOldValueBoundsAux
#     variable = 'bounds_dummy'
#     bounded_variable = 'd'
#     bound_type = lower
#   [../]
#   [./upper]
#     type = ConstantBoundsAux
#     variable = 'bounds_dummy'
#     bounded_variable = 'd'
#     bound_type = upper
#     bound_value = 1
#   [../]
# []
[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    component = 0
    displacements = 'disp_x disp_y disp_z'
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
    displacements = 'disp_x disp_y disp_z'
  [../]
  [./solid_z]
    type = ADStressDivergenceTensors
    variable = 'disp_z'
    component = 2
    displacements = 'disp_x disp_y disp_z'
  [../]
  [./pff_diff]
    type = ADPFFDiffusion
    variable = 'd'
  [../]
  [./pff_barrier]
    type = ADPFFBarrier
    variable = 'd'
  [../]
  [./pff_react]
    type = ADPFFReaction
    variable = 'd'
    driving_energy_uo = 'E_driving'
    lag = false
  [../]
[]
[BCs]
  [./upwardLoad]
    type = FunctionDirichletBC
    variable = 'disp_y'
    boundary = 'top'
    function = '0.05*t'
  [../]
  [./fixedCircleX]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'circle'
    value = 0
  [../]
  [./fixedCircleY]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'circle'
    value = 0
  [../]
  [./fixedZ]
    type = DirichletBC
    variable = 'disp_z'
    boundary = 'all_nodes'
    value = 0
  [../]
  [./noDamageOnBoundary]
   type = DirichletBC
   variable = 'd'
   boundary = 'circle, top, bottom, right, left'
   value = 0
  [../]
[]
[Materials]
  [./elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = ${E}
    poissons_ratio = ${nu}
  [../]
  [./strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y disp_z'
  [../]
  [./stress]
    type = SmallStrainDegradedElasticPK2Stress_StrainVolDev
    d = 'd'
    d_crit = ${dc}
    degradation_mat = 'g'
  [../]
  [./bulk]
    type = GenericConstantMaterial
    prop_names = 'phase_field_regularization_length energy_release_rate critical_fracture_energy'
    prop_values = '${l} ${Gc} ${psic}'
  [../]
  [./local_dissipation]
    type = QuadraticLocalDissipation
    d = 'd'
  [../]
  [./fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 2
  [../]
  [./degradation]
    type = QuadraticDegradation
    d = 'd'
    residual_degradation = ${k}
  [../]
  [./elastic_energy]
    type = ElasticEnergyDensity
  [../]
[]
[Executioner]
  type = FixedPointTransient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type'
  petsc_options_value = 'lu      ilu          200         200                0                     vinewtonrsls'
  dt = 1
  end_time = 8
  nl_abs_tol = 1e-06
  nl_rel_tol = 1e-06
  automatic_scaling = true
  compute_scaling_once = false
  fp_max_its = 0
  fp_tol = 1e-06
  accept_on_max_fp_iteration = true
[]
[Postprocessors]
  # [./helmholtz_energy]
  #   type = HelmholtzEnergy
  #   d = d
  #   pressure_mat = 'p'
  #   fracture_energy_name = 'fracture_energy'
  # [../]
  [./fracture_energy]
    type = FractureEnergy
    d = d
  [../]
[]
[Outputs]
  print_linear_residuals = true
  [./exodus]
    type = Exodus
    file_base = 'volDevDecomposition'
  [../]
  [./console]
    type = Console
    outlier_variable_norms = false
  [../]
[]

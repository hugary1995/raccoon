[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[MultiApps]
  [./damage]
    type = TransientMultiApp
    input_files = 'damage.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Transfers]
  [./disp_x]
    type = MultiAppCopyTransfer
    multi_app = damage
    direction = to_multiapp
    source_variable = 'disp_x'
    variable = 'disp_x'
  [../]
  [./disp_y]
    type = MultiAppCopyTransfer
    multi_app = damage
    direction = to_multiapp
    source_variable = 'disp_y'
    variable = 'disp_y'
  [../]
  [./d]
    type = MultiAppCopyTransfer
    multi_app = damage
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd'
  [../]
[]

[Mesh]
  type = FileMesh
  file = gold/film.msh
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./d]
  [../]
[]

[Modules]
  [./KLExpansion]
    file_name = 'gold/kl_info.txt'
    field_name = 'E_crit'
    field_distribution = GAMMA
    mean = 4e-4
    CV = 0.8
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
  [../]
[]

[Materials]
  [./eigen_strain]
    type = ComputeEigenstrainFromFunctionEigenstress
    initial_stress = 't 0 0 0 0 0 0 0 0'
    eigenstrain_name = is
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = 0.2
    youngs_modulus = 4
  [../]
  [./strain]
    type = ADComputeSmallStrain
    eigenstrain_names = is
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_NoSplit
    history = false
  [../]
  [./fracture_energy_barrier]
    type = GenericFunctionMaterial
    prop_names = 'b'
    prop_values = 'E_crit'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = 8e-4
    L = 0.075
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
  [../]
[]

[BCs]
  [./boundary_xdisp]
    type = DirichletBC
    variable = disp_x
    boundary = 'bottom left right'
    value = 0
  [../]
  [./boundary_ydisp]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  [../]
[]

[Postprocessors]
  [./driving]
    type = SideIntegralPositiveMaxPrincipalStressPostprocessor
    boundary = 'top'
  [../]
  [./integral_damage]
    type = SideIntegralVariablePostprocessor
    boundary = 'top'
    variable = d
  [../]
[../]

[VectorPostprocessors]
  [./top_damage_profile]
    type = LineValueSampler
    start_point = '0.0 1.0 0.0'
    end_point = '20.0 1.0 0.0'
    variable = 'd'
    num_points = 200
    sort_by = 'x'
    execute_on = FINAL
  [../]
[]

[UserObjects]
  [./termination]
    type = Terminator
    expression = 'driving < 0.2 & integral_damage > 0.0'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  nl_abs_tol = 1e-06
  nl_rel_tol = 1e-06

  dt = 1e-3
  dtmin = 1e-3

  picard_max_its = 100
  picard_abs_tol = 1e-06
  picard_rel_tol = 1e-01
[]

[Outputs]
  interval = 1
  print_linear_residuals = false
  [./exodus]
    type = Exodus
    file_base = output/out
  [../]
  [./csv]
    type = CSV
    file_base = output/postprocess
    execute_postprocessors_on = NONE
    execute_vector_postprocessors_on = FINAL
  [../]
[]

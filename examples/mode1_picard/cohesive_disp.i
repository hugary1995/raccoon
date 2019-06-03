[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[MultiApps]
  [./damage]
    type = TransientMultiApp
    input_files = 'cohesive_damage.i'
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
    execute_on = SAME_AS_MULTIAPP
  [../]
  [./disp_y]
    type = MultiAppCopyTransfer
    multi_app = damage
    direction = to_multiapp
    source_variable = 'disp_y'
    variable = 'disp_y'
    execute_on = SAME_AS_MULTIAPP
  [../]
  [./d]
    type = MultiAppCopyTransfer
    multi_app = damage
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd'
    execute_on = SAME_AS_MULTIAPP
  [../]
[]

[Mesh]
  type = FileMesh
  file = 'gold/geo.e'
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

[BCs]
  [./ydisp]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 2
    function = 't'
  [../]
  [./yfix]
    type = DirichletBC
    variable = disp_y
    boundary = 1
    value = 0
  [../]
  [./xfix]
    type = DirichletBC
    variable = disp_x
    boundary = '1 2'
    value = 0
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeElasticityTensor
    C_ijkl = '120.0 80.0'
    fill_method = symmetric_isotropic
  [../]
  [./strain]
    type = ADComputeSmallStrain
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_StrainSpectral
    history = false
  [../]
  [./fracture_energy_barrier]
    type = GenericFunctionMaterial
    prop_names = 'b'
    prop_values = '0.005'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = 1e-3
    L = 0.02
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  dt = 1e-5
  end_time = 5e-3

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-08

  picard_max_its = 100
  picard_abs_tol = 1e-06
  picard_rel_tol = 1e-1
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

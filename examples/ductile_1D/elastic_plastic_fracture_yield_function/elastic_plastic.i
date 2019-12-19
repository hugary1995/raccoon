[MultiApps]
  [./fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Transfers]
  [./send_E_el]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'E_el'
    variable = 'E_el'
  [../]
  [./send_d_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'd_ref'
    variable = 'd_ref'
  [../]
  [./get_d]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd'
  [../]
[]

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Mesh]
  type = GeneratedMesh
  dim = 3
  elem_type = HEX8
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
[]

[AuxVariables]
  [./load]
    family = SCALAR
  [../]
  [./d]
  [../]
  [./d_ref]
  [../]
  [./elastic_strain_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./plastic_strain_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./strain_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./elastic_strain_yy]
    type = MaterialRankTwoTensorAux
    variable = elastic_strain_yy
    i = 1
    j = 1
    property = elastic_strain
  [../]
  [./plastic_strain_yy]
    type = MaterialRankTwoTensorAux
    variable = plastic_strain_yy
    i = 1
    j = 1
    property = plastic_strain
  [../]
  [./strain_yy]
    type = MaterialRankTwoTensorAux
    variable = strain_yy
    i = 1
    j = 1
    property = eulerian_almansi_strain
  [../]
  [./stress_yy]
    type = MaterialRankTwoTensorAux
    variable = stress_yy
    i = 1
    j = 1
    property = cauchy_stress
  [../]
  [./E_el]
    type = MaterialRealAux
    property = 'fracture_driving_energy'
    variable = 'E_el'
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
  [./solid_z]
    type = ADStressDivergenceTensors
    variable = disp_z
    component = 2
  [../]
[]

[Materials]
  [./fracture_energy_barrier]
    type = CriticalFractureEnergy
    initial_local_dissipation_slope = 0
    initial_degradation_slope = -2
  [../]
  [./local_dissipation]
    type = QuadraticLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = 68.9
    L = 2
    local_dissipation_norm = 2
  [../]
  [./degradation]
    type = QuadraticDegradation
    d = d
    residual_degradation = 1e-12
  [../]

  [./elasticity_tensor]
    type = ComputeElasticityTensor
    C_ijkl = '68.8e3 0.33'
    fill_method = symmetric_isotropic_E_nu
  [../]
  [./strain]
    type = GreenStrain
  [../]
  [./elastic]
    type = CNHDegradedPK2ElastoPlasticStress
    yield_stress = 320
    W0 = 1e6
    d = d
    linear_hardening_coefficient = 688
    history = true
  [../]
[]

[BCs]
  [./ydisp]
    type = FunctionPresetBC
    variable = disp_y
    boundary = top
    function = 't'
  [../]
  [./yfix]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0
  [../]
  [./xfix]
    type = PresetBC
    variable = disp_x
    boundary = left
    value = 0
  [../]
  [./zfix]
    type = PresetBC
    variable = disp_z
    boundary = back
    value = 0
  [../]
[]

[Executioner]
  type = Transient

  solve_type = 'NEWTON'

  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  nl_rel_tol = 1e-08
  nl_abs_tol = 1e-10

  dtmin = 1e-5
  dt = 1e-5
  end_time = 0.4
[]

[Postprocessors]
  [./stress_yy]
    type = ElementAverageValue
    variable = stress_yy
  [../]
  [./plastic_strain_yy]
    type = ElementAverageValue
    variable = plastic_strain_yy
  [../]
  [./elastic_strain_yy]
    type = ElementAverageValue
    variable = elastic_strain_yy
  [../]
  [./strain_yy]
    type = ElementAverageValue
    variable = strain_yy
  [../]
  [./d]
    type = AverageNodalVariableValue
    variable = d
  [../]
[]

[Outputs]
  print_linear_residuals = false
  hide = 'load'
  exodus = true
  csv = true
[]

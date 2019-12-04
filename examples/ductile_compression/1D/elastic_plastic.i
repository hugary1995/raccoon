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
    type = GenericConstantMaterial
    prop_names = 'b'
    prop_values = '2.2'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = 45
    L = 2
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-12
  [../]

  [./elasticity_tensor]
    type = ComputeElasticityTensor
    C_ijkl = '1.1e5 0.343'
    fill_method = symmetric_isotropic_E_nu
  [../]
  [./strain]
    type = GreenStrain
  [../]
  [./elastic]
    type = CNHDegradedPK1ElastoPlasticStress_VolDev
    yield_stress = 300
    W0 = 200
    d = d
    linear_hardening_coefficient = 700
    history = true
  [../]
[]

[BCs]
  [./ydisp]
    type = ScalarPresetBC
    variable = disp_y
    boundary = top
    scalar_var = 'load'
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

  dtmin = 1e-4
  dt = 1e-4
  end_time = 0.8
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
  [./fracture_driving_energy]
    type = ElementAverageValue
    variable = E_el
  [../]
[]

[Outputs]
  print_linear_residuals = false
  hide = 'load'
  exodus = true
  csv = true
[]

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
  [./d]
  [../]
  [./elastic_strain_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./plastic_strain_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./green_strain_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./left_cauchy_green_strain_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./eulerian_almansi_strain_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./kirchhoff_stress_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./cauchy_stress_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./second_piola_kirchhoff_stress_yy]
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
  [./green_strain_yy]
    type = MaterialRankTwoTensorAux
    variable = green_strain_yy
    i = 1
    j = 1
    property = mechanical_strain
  [../]
  [./left_cauchy_green_strain_yy]
    type = MaterialRankTwoTensorAux
    variable = left_cauchy_green_strain_yy
    i = 1
    j = 1
    property = left_cauchy_green_strain
  [../]
  [./eulerian_almansi_strain]
    type = MaterialRankTwoTensorAux
    variable = eulerian_almansi_strain_yy
    i = 1
    j = 1
    property = eulerian_almansi_strain
  [../]
  [./cauchy_stress_yy]
    type = MaterialRankTwoTensorAux
    variable = cauchy_stress_yy
    i = 1
    j = 1
    property = cauchy_stress
  [../]
  [./kirchhoff_stress_yy]
    type = MaterialRankTwoTensorAux
    variable = kirchhoff_stress_yy
    i = 1
    j = 1
    property = kirchhoff_stress
  [../]
  [./second_piola_kirchhoff_stress_yy]
    type = MaterialRankTwoTensorAux
    variable = second_piola_kirchhoff_stress_yy
    i = 1
    j = 1
    property = second_piola_kirchhoff_stress
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
    type = FractureEnergyBarrier
    initial_local_dissipation_slope = 0
    initial_degradation_slope = -2
  [../]
  [./local_dissipation]
    type = QuadraticLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = 6.88e4
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
    W0 = 0
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
  dt = 1e-3
  end_time = 0.4
[]

[Postprocessors]
  [./plastic_strain_yy]
    type = ElementAverageValue
    variable = plastic_strain_yy
  [../]
  [./elastic_strain_yy]
    type = ElementAverageValue
    variable = elastic_strain_yy
  [../]
  [./green_strain_yy]
    type = ElementAverageValue
    variable = green_strain_yy
  [../]
  [./left_cauchy_green_strain_yy]
    type = ElementAverageValue
    variable = left_cauchy_green_strain_yy
  [../]
  [./eulerian_almansi_strain_yy]
    type = ElementAverageValue
    variable = eulerian_almansi_strain_yy
  [../]
  [./cauchy_stress_yy]
    type = ElementAverageValue
    variable = cauchy_stress_yy
  [../]
  [./kirchhoff_stress_yy]
    type = ElementAverageValue
    variable = kirchhoff_stress_yy
  [../]
  [./second_piola_kirchhoff_stress_yy]
    type = ElementAverageValue
    variable = second_piola_kirchhoff_stress_yy
  [../]
[]

[Outputs]
  print_linear_residuals = false
  exodus = true
  csv = true
[]

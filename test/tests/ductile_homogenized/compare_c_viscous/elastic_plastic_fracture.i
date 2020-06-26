[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Problem]
  type = FixedPointProblem
[]

[Mesh]
  type = GeneratedMesh
  dim = 3
  elem_type = 'HEX8'
  xmax = 1000
  ymax = 1000
  zmax = 1000
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

[AuxVariables]
  [./bounds_dummy]
  [../]
  [./elastic_strain_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./plastic_strain_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./total_strain_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./E_elastic]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./W_plastic]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./ep]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[UserObjects]
  [./E_el_active]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'E_el_active'
  [../]
  [./W_pl]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'W_pl'
  [../]
  [./E_coalesce]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'E_coalesce'
  [../]
[]

[Bounds]
  [./irreversibility]
    type = VariableOldValueBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = lower
  [../]
  [./upper]
    type = ConstantBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = upper
    bound_value = 1
  [../]
[]

[AuxKernels]
  [./elastic_strain_yy]
    type = ADMaterialRankTwoTensorAux
    variable = 'elastic_strain_yy'
    property = 'elastic_strain'
    i = 1
    j = 1
  [../]
  [./plastic_strain_yy]
    type = ADMaterialRankTwoTensorAux
    variable = 'plastic_strain_yy'
    property = 'plastic_strain'
    i = 1
    j = 1
  [../]
  [./total_strain_yy]
    type = ADMaterialRankTwoTensorAux
    variable = 'total_strain_yy'
    property = 'mechanical_strain'
    i = 1
    j = 1
  [../]
  [./stress_yy]
    type = ADMaterialRankTwoTensorAux
    variable = 'stress_yy'
    property = 'cauchy_stress'
    i = 1
    j = 1
  [../]
  [./E_elastic]
    type = ADMaterialRealAux
    variable = 'E_elastic'
    property = 'E_el_active'
  [../]
  [./W_plastic]
    type = ADMaterialRealAux
    variable = 'W_plastic'
    property = 'W_pl'
  [../]
  [./ep]
    type = ADMaterialRealAux
    variable = 'ep'
    property = 'effective_plastic_strain'
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    component = 0
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
  [../]
  [./solid_z]
    type = ADStressDivergenceTensors
    variable = 'disp_z'
    component = 2
  [../]
  [./visco]
    type = CoefTimeDerivative
    variable = 'd'
    Coefficient = 200
  [../]
  [./pff_diff]
    type = ADPFFDiffusion
    variable = 'd'
  [../]
  [./pff_barr]
    type = ADPFFBarrier
    variable = 'd'
  [../]
  [./pff_barr_coalesce]
    type = ADPFFBarrier
    variable = 'd'
    mobility_name = 'coalescence_mobility'
  [../]
  [./pff_react_elastic]
    type = ADPFFReaction
    variable = 'd'
    degradation_name = 'g'
    driving_energy_uo = 'E_el_active'
    lag = false
  [../]
  [./pff_react_plastic]
    type = ADPFFReaction
    variable = 'd'
    degradation_name = 'gp'
    driving_energy_uo = 'W_pl'
    lag = false
  [../]
  [./pff_react_coalesce]
    type = ADPFFReaction
    variable = 'd'
    degradation_name = 'gp'
    driving_energy_uo = 'E_coalesce'
    lag = false
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 6.88e4
    poissons_ratio = 0.33
  [../]
  [./strain]
    type = GreenStrain
  [../]
  [./elastic]
    type = CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence
    d = 'd'
    yield_stress = 320
    slope = 688
    coalescence_coefficient = ${c}
  [../]

  [./bulk]
    type = GenericConstantMaterial
    prop_names = 'phase_field_regularization_length energy_release_rate critical_fracture_energy'
    prop_values = '4000 1.38e5 2'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = 'd'
  [../]
  [./fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = 'd'
    p = 1
  [../]
  [./plastic_degradation]
    type = LorentzDegradation
    d = 'd'
    p = 1
    degradation_name = 'gp'
  [../]
  [./coalescence]
    type = CoalescenceEnergy
    coalescence_coefficient = ${c}
    degradation_name = 'gp'
    mobility_name = 'coalescence_mobility'
    coalescence_energy_name = 'E_coalesce'
  [../]
[]

[BCs]
  [./ydisp]
    type = FunctionDirichletBC
    variable = 'disp_y'
    boundary = 'top'
    function = 't'
  [../]
  [./yfix]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'bottom'
    value = 0
  [../]
  [./xfix]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'left'
    value = 0
  [../]
  [./zfix]
    type = DirichletBC
    variable = 'disp_z'
    boundary = 'back'
    value = 0
  [../]
[]

[Executioner]
  type = FixedPointTransient

  solve_type = 'NEWTON'

  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type'
  petsc_options_value = 'asm      lu          1000         200                0                     vinewtonrsls'

  nl_rel_tol = 1e-08
  nl_abs_tol = 1e-10

  dtmin = 0.1
  dt = 0.1
  end_time = 100
  timestep_tolerance = 1e-10

  fp_max_its = 200
  fp_tol = 1e-06
  accept_on_max_fp_iteration = true

  automatic_scaling = true
[]

[Postprocessors]
  [./total_strain_yy]
    type = ElementAverageValue
    variable = 'total_strain_yy'
  [../]
  [./stress_yy]
    type = ElementAverageValue
    variable = 'stress_yy'
  [../]
  [./d]
    type = ElementAverageValue
    variable = 'd'
  [../]
  [./ep]
    type = ElementAverageValue
    variable = 'ep'
  [../]
[]

[Outputs]
  print_linear_residuals = false
  [./csv]
    type = CSV
    file_base = 'stress_strain_c_${c}'
    time_column = false
    delimiter = ','
  [../]
[]

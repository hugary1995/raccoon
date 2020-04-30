[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Problem]
  type = FixedPointProblem
[]

[Mesh]
   [./fmg]
     type = FileMeshGenerator
     file = 'gold/innermesh.msh'
   [../]
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./d]
  [../]
[]

[AuxVariables]
  [./accel_x]
  [../]
  [./accel_y]
  [../]
  [./pre_wave]
  [../]
  [./stress_h]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_VonMises]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./bounds_dummy]
  [../]
[]

[UserObjects]
  [./E_driving]
    type = FPIMaterialPropertyUserObject
    mat_prop = 'E_el_active'
  [../]
[]

[Bounds]
  [./irreversibility]
    type = VariableOldValueBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = lower
  [../]
  [./upper_bound]
    type = ConstantBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_value = 1
  [../]
[]

[Kernels]
  [./solid_x]
     type = ADStressDivergenceExpTensors
     variable = 'disp_x'
     displacements = 'disp_x disp_y'
     component = 0
  [../]
  [./solid_y]
     type = ADStressDivergenceExpTensors
     variable = 'disp_y'
     displacements = 'disp_x disp_y'
     component = 1
  [../]
  [./inertia_x]
    type = InertialForceExp
    variable = 'disp_x'
  [../]
  [./inertia_y]
    type = InertialForceExp
    variable = 'disp_y'
  [../]
  [./pff_diff]
    type = PhaseFieldFractureEvolutionDiffusion
    variable = 'd'
  [../]
  [./pff_barr]
    type = PhaseFieldFractureEvolutionBarrier
    variable = 'd'
  [../]
  [./pff_react]
    type = PhaseFieldFractureEvolutionReaction
    variable = 'd'
    driving_energy_uo = 'E_driving'
    lag = false
  [../]
[]

[AuxKernels]
  [./accel_x]
    type = ExpAccelAux
    variable = 'accel_x'
    displacement = 'disp_x'
    execute_on = 'TIMESTEP_END'
  [../]
  [./accel_y]
    type = ExpAccelAux
    variable = 'accel_y'
    displacement = 'disp_y'
    execute_on = 'TIMESTEP_END'
  [../]
  # [./stress_h]
  #   type = RankTwoScalarAux
  #   rank_two_tensor = 'stress'
  #   variable = 'stress_h'
  #   scalar_type = Hydrostatic
  #   execute_on = 'TIMESTEP_END'
  # [../]
  # [./stress_VonMises]
  #   type = RankTwoScalarAux
  #   rank_two_tensor = 'stress'
  #   variable = 'stress_VonMises'
  #   scalar_type = VonMisesStress
  #   execute_on = 'TIMESTEP_END'
  # [../]
[]

[BCs]
  [./top_y]
    type = CoupledNeumannBC
    variable = 'disp_y'
    boundary = 'top'
    coupled_var = 'pre_wave'
  [../]
  [./right_x]
    type = CoupledNeumannBC
    variable = 'disp_x'
    boundary = 'right'
    coupled_var = 'pre_wave'
  [../]
  [./left_x]
    type = CoupledNeumannBC
    variable = 'disp_x'
    boundary = 'left'
    coupled_var = 'pre_wave'
  [../]
  [./bottom_y]
    type = CoupledNeumannBC
    variable = 'disp_y'
    boundary = 'bottom'
    coupled_var = 'pre_wave'
  [../]
[]

[Materials]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '1.995e-3'
  [../]
  [./Elasticity_tensor]
    type = ComputeElasticityTensor
    fill_method = symmetric_isotropic
    C_ijkl = '0.01305 0.01073'
  [../]
  [./strain]
    type = ADComputeSmallStrain
  [../]
  [./stress]
    type = SmallStrainDegradedElasticPK2Stress_StrainVolDev
    d = 'd'
    d_crit = 2.0
  [../]

  [./bulk]
    type = GenericConstantMaterial
    prop_names = 'phase_field_regularization_length energy_release_rate critical_fracture_energy'
    prop_values = '0.004 5.226e-10 1.8e-9'
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
    residual_degradation = 1e-09
  [../]
[]

[Executioner]
  type = FixedPointTransient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap -snes_type'
  petsc_options_value = 'asm      31                  preonly       lu           1               vinewtonrsls'

  start_time = 0
  end_time = 3.0
  l_max_its = 50
  nl_max_its = 20

  l_tol = 1e-12
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8
  dt = 1e-3

  fp_max_its = 2
  fp_tol = 1e-06
  accept_on_max_fp_iteration = true

  automatic_scaling = true
[]

[Outputs]
  [./exodus]
    type = Exodus
    file_base = DispOut
  [../]
[]

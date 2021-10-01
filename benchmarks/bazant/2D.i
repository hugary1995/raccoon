E = 1e-6
rho = 1e-12
nu = 0.3

psic = 1.125e-6
Gc = 0.45e-3
l = 30

[Problem]
  type = FixedPointProblem
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gmg]
    type = FileMeshGenerator
    file = 'mesh/geo.msh'
  []
[]

[UserObjects]
  [E_el_active]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'E_el_active'
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [d]
  []
[]

[AuxVariables]
  [stress_xx]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_xy]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_yy]
    order = CONSTANT
    family = MONOMIAL
  []
  [hmin]
    order = CONSTANT
    family = MONOMIAL
  []
  [hmax]
    order = CONSTANT
    family = MONOMIAL
  []
  [bounds_dummy]
  []
  [accel_x]
  []
  [accel_y]
  []
  [vel_x]
  []
  [vel_y]
  []
[]

[Bounds]
  [irreversibility]
    type = VariableOldValueBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = lower
  []
  [upper]
    type = ConstantBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = upper
    bound_value = 1
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
  [inertia_x]
    type = InertialForce
    variable = disp_x
    density = 'reg_density'
  []
  [inertia_y]
    type = InertialForce
    variable = disp_y
    density = 'reg_density'
  []

  [pff_diff]
    type = ADPFFDiffusion
    variable = 'd'
  []
  [pff_barr]
    type = ADPFFBarrier
    variable = 'd'
  []
  [pff_react]
    type = ADPFFReaction
    variable = 'd'
    driving_energy_uo = 'E_el_active'
  []
[]

[AuxKernels]
  [stressxx]
    type = ADRankTwoAux
    rank_two_tensor = stress
    variable = stress_xx
    index_i = 0
    index_j = 0
    execute_on = TIMESTEP_END
  []
  [stressxy]
    type = ADRankTwoAux
    rank_two_tensor = stress
    variable = stress_xy
    index_i = 0
    index_j = 1
    execute_on = TIMESTEP_END
  []
  [stressyy]
    type = ADRankTwoAux
    rank_two_tensor = stress
    variable = stress_yy
    index_i = 1
    index_j = 1
    execute_on = TIMESTEP_END
  []
  [min_h]
    type = ElementLengthAux
    variable = 'hmin'
    method = min
    execute_on = 'INITIAL'
  []
  [max_h]
    type = ElementLengthAux
    variable = 'hmax'
    method = max
    execute_on = 'INITIAL'
  []
  [ax]
    type = NewmarkAccelAux
    variable = 'accel_x'
    displacement = 'disp_x'
    velocity = 'vel_x'
    beta = 0.25
    execute_on = 'TIMESTEP_END'
  []
  [ay]
    type = NewmarkAccelAux
    variable = 'accel_y'
    displacement = 'disp_y'
    velocity = 'vel_y'
    beta = 0.25
    execute_on = 'TIMESTEP_END'
  []
  [vx]
    type = NewmarkVelAux
    variable = 'vel_x'
    acceleration = 'accel_x'
    gamma = 0.5
    execute_on = 'TIMESTEP_END'
  []
  [yx]
    type = NewmarkVelAux
    variable = 'vel_y'
    acceleration = 'accel_y'
    gamma = 0.5
    execute_on = 'TIMESTEP_END'
  []
[]

[Materials]
  [const]
    type = ADGenericConstantMaterial
    prop_names = 'density phase_field_regularization_length energy_release_rate '
                 'critical_fracture_energy'
    prop_values = '${rho} ${l} ${Gc} ${psic}'
  []
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = '${E}'
    poissons_ratio = '${nu}'
  []
  [stress]
    type = SmallStrainDegradedElasticPK2Stress_StrainSpectral
    d = 'd'
  []
  [strain]
    type = ADComputeSmallStrain
  []
  [local_dissipation]
    type = LinearLocalDissipation
    d = 'd'
  []
  [fracture_properties]
    type = ADFractureMaterial
    local_dissipation_norm = 8/3
  []
  [degradation]
    type = LorentzDegradation
    d = 'd'
    residual_degradation = 0
  []
  [reg_density]
    type = MaterialConverter
    ad_props_in = 'density'
    reg_props_out = 'reg_density'
  []
[]

[BCs]
  [left]
    type = ADPressure
    boundary = left
    variable = 'disp_x'
    component = 0
    function = 'if(t < 1, -1e-6, 0)'
  []
  [right]
    type = ADPressure
    boundary = right
    variable = 'disp_x'
    component = 0
    function = 'if(t < 1, -1e-6, 0)'
  []
  [fix_y]
    type = ADDirichletBC
    variable = 'disp_y'
    boundary = 'center left right'
    value = 0.0
  []
[]

[Postprocessors]
  [strain_energy]
    type = ADStrainEnergy
  []
  [active_strain_energy]
    type = ADElementIntegralMaterialProperty
    mat_prop = 'E_el_active'
  []
  [kinetic_energy]
    type = ADKineticEnergy
  []
  [fracture_energy]
    type = ADFractureEnergy
    d = 'd'
  []
  [fpi]
    type = FPIterations
  []
  [explicit_dt]
    type = ADBetterCriticalTimeStep
    density_name = 'density'
    execute_on = 'INITIAL TIMESTEP_BEGIN TIMESTEP_END'
  []
[]

[Executioner]
  type = FixedPointTransient
  dt = 50e-3
  num_steps = 400
  solve_type = 'NEWTON'

  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'

  nl_abs_tol = 1e-6
  nl_rel_tol = 1e-8
  l_max_its = 50
  nl_max_its = 100

  accept_on_max_fp_iteration = false
  fp_max_its = 1000
  fp_tol = 1e-6

  [TimeIntegrator]
    type = NewmarkBeta
  []
[]

[Outputs]
  print_linear_residuals = false
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
  [Exodus]
    type = Exodus
    file_base = 'mechanical_fracture_2d'
    output_material_properties = true
    show_material_properties = 'E_el_active'
  []
  [Console]
    type = Console
    outlier_variable_norms = false
    interval = 10
  []
  [CSV]
    type = CSV
    file_base = 'mechanical_fracture_2d_energies'
  []
[]

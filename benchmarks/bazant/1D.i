E = 1
rho = 1
nu = 0

# sigmac = 1.5
psic = 1.125
Gc = 0.45
l = 0.05

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 1
    xmin = -1
    xmax = 1
    nx = 400
  []
[]

[Problem]
  type = FixedPointProblem
[]

[GlobalParams]
  displacements = 'disp_x'
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
  [d]
  []
[]

[AuxVariables]
  [stress]
    order = CONSTANT
    family = MONOMIAL
  []
  [bounds_dummy]
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
  [inertia_x]
    type = InertialForce
    variable = disp_x
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
    type= ADPFFReaction
    variable = 'd'
    driving_energy_uo = 'E_el_active'
  []
[]

[AuxKernels]
  [stress]
    type = ADRankTwoAux
    rank_two_tensor = stress
    variable = stress
    index_i = 0
    index_j = 0
  []
[]

[Materials]
  [const]
    type = GenericConstantMaterial
    prop_names = 'density phase_field_regularization_length energy_release_rate critical_fracture_energy'
    prop_values = '${rho} ${l} ${Gc} ${psic}'
  []
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = '${E}'
    poissons_ratio = '${nu}'
  []
  [stress]
    type = SmallStrainDegradedElasticPK2Stress_NoSplit
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
    type = FractureMaterial
    local_dissipation_norm = 8/3
  []
  [degradation]
    type = LorentzDegradation
    d = 'd'
    residual_degradation = 0
  []
  [reg_elasticity_tensor]
    type = MaterialConverter
    ad_props_in = 'effective_stiffness'
    reg_props_out = 'reg_effective_stiffness'
  []
[]

[BCs]
  [left]
    type = ADPressure
    boundary = left
    variable = 'disp_x'
    component = 0
    constant = '-1'
  []
  [right]
    type = ADPressure
    boundary = right
    variable = 'disp_x'
    component = 0
    constant = '-1'
  []
[]

[Postprocessors]
  [elastic_energy] # The degraded energy
    type = StrainEnergy
  []
  [strain_energy] # The in-tact energy
    type = ADElementIntegralMaterialProperty
    mat_prop = 'E_el_active'
  []
  [kinetic_energy]
    type = KineticEnergy
  []
  [fracture_energy]
    type = FractureEnergy
    d = 'd'
  []
  [fpi]
    type = FPIterations
  []
  [explicit_dt]
    type = BetterCriticalTimeStep
    density_name = 'density'
    E_name = 'reg_effective_stiffness'
    execute_on = 'INITIAL TIMESTEP_BEGIN TIMESTEP_END'
  []
[]

[Executioner]
  type = FixedPointTransient
  # dt = 1e-3 #CFL condition
  dt = 0.005
  num_steps = 1000
  solve_type = 'NEWTON'

  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'

  nl_abs_tol = 1e-6
  nl_rel_tol = 1e-8
  l_max_its = 50
  nl_max_its = 100

  fp_max_its = 10
  fp_tol = 1e-2

  [TimeIntegrator]
    type = NewmarkBeta
  []
[]

[Outputs]
  print_linear_residuals = false
  [Exodus]
    type = Exodus
    file_base = 'mechanical_fracture1d'
    output_material_properties = true
    show_material_properties = 'E_el_active'
  []
  [Console]
    type = Console
    outlier_variable_norms = false
    interval = 10
  []
  [csv]
    type = CSV
    file_base = 'mechanical_fracture1d_energies'
  []
[]

[Debug]
  # show_var_residual_norms = true
  # show_parser = true
  # show_actions = true
[]

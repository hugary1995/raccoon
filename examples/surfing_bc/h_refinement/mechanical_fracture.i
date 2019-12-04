[Mesh]
  type = FileMesh
  file = 'gold/${c}.msh'
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
  [./bounds_dummy]
  [../]
  [./a]
    order = FIRST
    family = SCALAR
  [../]
  [./fx]
  [../]
  [./fy]
  [../]
  [./alpha]
    family = MONOMIAL
  [../]
[]

[ICs]
  [./d]
    type = CohesiveDamageIC
    variable = d
    d0 = 1
    l = 0.03
    x1 = 0
    y1 = 0
    z1 = 0
    x2 = 0.2
    y2 = 0
    z2 = 0
  [../]
[]

[AuxScalarKernels]
  [./a]
    type = FunctionScalarAux
    variable = a
    function = 't'
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
[]

[AuxKernels]
  [./alpha]
    type = MaterialRealAux
    property = 'alpha'
    variable = 'alpha'
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Bounds]
  [./irreversibility]
    type = Irreversibility
    variable = bounds_dummy
    bounded_variable = d
    upper = 1
    lower = 'd'
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    displacements = 'disp_x disp_y'
    save_in = 'fx'
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    displacements = 'disp_x disp_y'
    save_in = 'fy'
  [../]
  [./fracture]
    type = PhaseFieldFractureEvolution
    variable = d
    driving_energy_name = E_el
  [../]
[]

[BCs]
  [./x]
    type = ModeISurfingDirichletBC
    variable = disp_x
    boundary = 'top left right'
    component = 0
    E = 2.1e5
    nu = 0.3
    Gc = 2.7
    crack_tip_x = a
    crack_tip_y = 0
  [../]
  [./y]
    type = ModeISurfingDirichletBC
    variable = disp_y
    boundary = 'top left right'
    component = 1
    E = 2.1e5
    nu = 0.3
    Gc = 2.7
    crack_tip_x = a
    crack_tip_y = 0
  [../]
  [./symmetry]
    type = DirichletBC
    variable = disp_y
    value = 0
    boundary = 'bottom'
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.1e5
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = GreenStrain
    displacements = 'disp_x disp_y'
  [../]
  [./elastic]
    type = CNHDegradedPK1ElastoPlasticStress_VolDev
    d = d
    history = false
    W0 = 0
    linear_hardening_coefficient = 1e5
    yield_stress = 1000
  [../]
  [./fracture_properties]
    type = GenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length b'
    prop_values = '2.7 0.03 14.88'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./phase_field_properties]
    type = FractureMaterial
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-09
  [../]
[]

[Postprocessors]
  [./fracture_energy]
    type = FractureEnergy
    d = d
  [../]
  [./surface_area]
    type = FractureSurfaceArea
    d = d
  [../]
  [./J]
    type = PhaseFieldJIntegral
    J_direction = '1 0 0'
    boundary = 'top left right'
    displacements = 'disp_x disp_y'
    elastic_energy_name = 'degraded_elastic_energy'
  [../]
  [./external_energy]
    type = ExternalEnergy
    boundary = 'top bottom left right'
    displacements = 'disp_x disp_y'
    forces = 'fx fy'
  [../]
  [./plastic_work]
    type = ElementIntegralMaterialProperty
    mat_prop = 'degraded_plastic_work'
  [../]
  [./elastic_energy]
    type = ElementIntegralMaterialProperty
    mat_prop = 'degraded_elastic_energy'
  [../]
[]

# [Preconditioning]
#   [./FSP]
#     type = FSP
#     topsplit = 'ud'
#     [./ud]
#       splitting = 'u d'
#       splitting_type  = additive
#     [../]
#     [./u]
#       vars = 'disp_x disp_y'
#       petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
#       petsc_options_value = 'asm      ilu          1000        200                0                    '
#     [../]
#     [./d]
#       vars = 'd'
#       petsc_options_iname = '-pc_type -snes_type'
#       petsc_options_value = 'lu       vinewtonrsls'
#     [../]
#   [../]
# []

[Executioner]
  type = TransientSupNorm
  solve_type = 'NEWTON'
  # petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  # petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type'
  petsc_options_value = 'asm      ilu          1000        200                0                     vinewtonrsls'
  dt = 5e-3
  start_time = 0.1
  end_time = 1.0

  nl_abs_tol = 1e-12
  nl_rel_tol = 1e-08

  automatic_scaling = true
  compute_scaling_once = false
[]

[Outputs]
  file_base = '${c}'
  print_linear_residuals = false
  hide = 'a'
  csv = true
  exodus = true
[]

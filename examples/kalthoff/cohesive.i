[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = FileMesh
  file = 'gold/mesh.msh'
[]

[Adaptivity]
  cycles_per_step = 4
  marker = crack
  max_h_level = 3
  [./Markers]
    [./crack]
      type = FractureMarker
      damage_fields = 'd'
    [../]
  [../]
[]

[Modules]
  [./PhaseFieldFracture]
    [./CohesiveFracture]
      [./d]
        viscosity = 0
        Gc = 22.2
        L = 0.35
        p = 1
        fracture_energy_barrier = 7.9
        residual_degradation = 1e-12
      [../]
    [../]
  [../]
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./dummy]
  [../]
  [./stress]
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./stress]
    type = RankTwoScalarAux
    variable = stress
    rank_two_tensor = stress
    scalar_type = MaxPrincipal
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Kernels]
  [./solid_x]
    type = PiolaKirchhoffStressDivergence
    variable = disp_x
    component = 0
    damage_fields = 'd'
  [../]
  [./solid_y]
    type = PiolaKirchhoffStressDivergence
    variable = disp_y
    component = 1
    damage_fields = 'd'
  [../]
  [./inertia_x]
    type = InertialForce
    variable = disp_x
    use_displaced_mesh = false
  [../]
  [./inertia_y]
    type = InertialForce
    variable = disp_y
    use_displaced_mesh = false
  [../]
[]

[Bounds]
  [./d]
    type = BoundsAux
    bounded_variable = d
    variable = dummy
    upper = 1.0
    lower = 0.0
  [../]
[]

[BCs]
  [./impact]
    type = FunctionPresetBC
    variable = disp_x
    boundary = 'impact'
    function = 'if( t<1e-6, 0.5*1.65e10*t*t, 1.65e4*t-0.5*1.65e-2)'
  [../]
[]

[Materials]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = 8e-9
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1.9e5
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = GreenStrain
  [../]
  [./stress]
    type = SmallStrainElasticDegradedStress
    damage_fields = 'd'
    history = true
  [../]
  [./degradation]
    type = LumpedDegradation
    damage_fields = 'd'
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient

  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu vinewtonrsls superlu_dist'

  end_time = 8.5e-5
  dt = 1e-7

  nl_rel_tol = 1e-08

  [./TimeIntegrator]
    type = NewmarkBeta
    beta = 0.25
    gamma = 0.5
  [../]
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
  interval = 1
[]

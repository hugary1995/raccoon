[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = FileMesh
  file = gold/film.msh
[]

[Modules]
  [./KLExpansion]
    file_name = 'gold/kl_info.txt'
    field_name = 'E_crit'
    field_distribution = GAMMA
    mean = 4e-4
    CV = 0.3
  [../]
  [./TensorMechanics]
    [./Master]
      [./all]
        add_variables = true
        strain = SMALL
        eigenstrain_names = is
        additional_generate_output = max_principal_stress
      [../]
    [../]
  [../]
  [./PhaseFieldFracture]
    [./CohesiveFracture]
      [./d]
        Gc = 8e-4
        L = 0.075
        fracture_energy_barrier = E_crit
      [../]
    [../]
    [./ElasticCoupling]
      [./all]
        damage_fields = 'd'
        strain = SMALL
        decomposition = STRAIN_SPECTRAL
        irreversibility = HISTORY
      [../]
    [../]
  [../]
[]

[Materials]
  [./eigen_strain]
    type = ComputeEigenstrainFromFunctionEigenstress
    initial_stress = 't 0 0 0 0 0 0 0 0'
    eigenstrain_name = is
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = 0.2
    youngs_modulus = 4
  [../]
[]

[BCs]
  [./boundary_xdisp]
    type = DirichletBC
    variable = disp_x
    boundary = 'bottom'
    value = 0
  [../]
  [./boundary_ydisp]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  [../]
  [./Periodic]
    [./ux]
      variable = disp_x
      auto_direction = 'x'
    [../]
    [./uy]
      variable = disp_y
      auto_direction = 'x'
    [../]
    [./d]
      variable = d
      auto_direction = 'x'
    [../]
  [../]
[]

[Postprocessors]
  [./driving]
    type = SideIntegralPositiveMaxPrincipalStressPostprocessor
    boundary = 'top'
  [../]
  [./integral_damage]
    type = SideIntegralVariablePostprocessor
    boundary = 'top'
    variable = d
  [../]
[../]

[VectorPostprocessors]
  [./top_damage_profile]
    type = LineValueSampler
    start_point = '0.0 1.0 0.0'
    end_point = '20.0 1.0 0.0'
    variable = 'd'
    num_points = 200
    sort_by = 'x'
    execute_on = FINAL
  [../]
[]

[UserObjects]
  [./termination]
    type = Terminator
    expression = 'driving < 0.2 & integral_damage > 0.0'
  [../]
[]

[Preconditioning]
  [./smp]
    type = SMP
    full = true
    ksp_norm = default
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -snes_linesearch_type -sub_pc_factor_levels'
  petsc_options_value = 'asm      ilu          200         200                basic                 0'
  nl_abs_tol = 1e-6
  nl_rel_tol = 1e-6
  l_max_its = 100
  nl_max_its = 50
  dt = 1e-3
  dtmin = 1e-3
[]

[Outputs]
  interval = 1
  print_linear_residuals = false
  [./exodus]
    type = Exodus
    file_base = output/out
  [../]
  [./csv]
    type = CSV
    file_base = output/postprocess
    execute_postprocessors_on = NONE
    execute_vector_postprocessors_on = FINAL
  [../]
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = FileMesh
  file = crack_mesh.e
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [c]
  []
[]

[AuxVariables]
  [stress_yy]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[Modules]
  [TensorMechanics]
    [Master]
      [All]
        add_variables = true
        strain = SMALL
        additional_generate_output = 'strain_yy stress_yy stress_xx'
        planar_formulation = PLANE_STRAIN
      []
    []
  []
[]

[Kernels]
  [pfbulk]
    type = AllenCahn
    variable = c
    mob_name = M
    f_name = F
  []
  [solid_x]
    type = PhaseFieldFractureMechanicsOffDiag
    variable = disp_x
    component = 0
    c = c
  []
  [solid_y]
    type = PhaseFieldFractureMechanicsOffDiag
    variable = disp_y
    component = 1
    c = c
  []
  [dcdt]
    type = TimeDerivative
    variable = c
  []
  [acint]
    type = ACInterface
    variable = c
    mob_name = M
    kappa_name = kappa_c
  []
[]

[AuxKernels]
  [stress_yy]
    type = RankTwoAux
    variable = stress_yy
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
    execute_on = timestep_end
  []
[]

[BCs]
  [ydisp]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 2
    function = 't'
    preset = false
  []
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = '1 2'
    value = 0
  []
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = 1
    value = 0
  []
[]

[Materials]
  [pfbulkmat]
    type = GenericConstantMaterial
    prop_names = 'gc_prop l visco M'
    prop_values = '1e-3 5e-3 1e-4 1e6'
  []
  [c_06]
    type = ParsedMaterial
    args = 'c'
    f_name = c_06
    function = 'if(c>0.6, 1, 0)'
  []
  [damage_stress]
    type = ComputeLinearElasticPFFractureStress
    c = c
    E_name = 'elastic_energy'
    D_name = 'degradation'
    F_name = 'local_fracture_energy'
    decomposition_type = strain_vol_dev
    use_current_history_variable = true
  []
  [degradation]
    type = DerivativeParsedMaterial
    f_name = degradation
    args = 'c'
    function = '(1.0-c)^2*(1.0 - eta) + eta'
    constant_names = 'eta'
    constant_expressions = '1.0e-6'
    derivative_order = 2
  []
  [local_fracture_energy]
    type = DerivativeParsedMaterial
    f_name = local_fracture_energy
    args = 'c'
    material_property_names = 'gc_prop l'
    function = 'c^2 * gc_prop / 2 / l'
    derivative_order = 2
  []
  [fracture_driving_energy]
    type = DerivativeSumMaterial
    args = c
    sum_materials = 'elastic_energy local_fracture_energy'
    derivative_order = 2
    f_name = F
  []
  [kappa_c]
    type = ParsedMaterial
    f_name = kappa_c
    material_property_names = 'gc_prop l'
    function = 'gc_prop*l'
  []
  [elasticity_tensor]
    type = ComputeElasticityTensor
    C_ijkl = '120.0 80.0'
    fill_method = symmetric_isotropic
  []
[]

[Preconditioning]
  active = 'smp'
  [smp]
    type = SMP
    full = true
  []
[]

[Postprocessors]
  [disp_y_top]
    type = SideAverageValue
    variable = disp_y
    boundary = 2
  []
  [stressyy]
    type = ElementAverageValue
    variable = stress_yy
  []
  [Total_crack]
    type = ElementIntegralMaterialProperty
    mat_prop = c_06
  []
[]

[Executioner]
  type = Transient

  solve_type = PJFNK

  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu superlu_dist'

  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-8
  l_max_its = 30
  nl_max_its = 50

  dt = 2e-6
  dtmin = 1e-10
  start_time = 0.0
  end_time = 0.025
  [Adaptivity]
    initial_adaptivity = 4
    max_h_level = 4
    refine_fraction = 0.95
    coarsen_fraction = 0.05
  []
[]

[Outputs]
  file_base = ModeIIstressvoldev
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
  interval = 30
  exodus = true
  csv = true
  gnuplot = true
[]

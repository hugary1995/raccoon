[Mesh]
  type = FileMesh
  file = 'gold/geo.e'
[]

[Variables]
  [./d]
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[Kernels]
  [./ACBulk]
    type = AllenCahn
    variable = d
    f_name = 'D_d'
    mob_name = 'M_d'
  [../]
  [./ACInterface]
    type = ACInterface
    variable = d
    kappa_name = 'kappa_d'
    mob_name = 'M_d'
  [../]
  [./TensorMechanics]
    displacements = 'disp_x disp_y'
  [../]
  [./offdiag_x]
    type = PhaseFieldFractureMechanicsOffDiag
    component = 0
    variable = disp_x
    c = d
  [../]
  [./offdiag_y]
    type = PhaseFieldFractureMechanicsOffDiag
    component = 1
    variable = disp_y
    c = d
  [../]
[]

[BCs]
  [./ydisp]
    type = FunctionPresetBC
    variable = disp_y
    boundary = 2
    function = 't'
  [../]
  [./yfix]
    type = PresetBC
    variable = disp_y
    boundary = 1
    value = 0
  [../]
  [./xfix]
    type = PresetBC
    variable = disp_x
    boundary = '1 2'
    value = 0
  [../]
[]

[Materials]
  [./bulk]
    type = FractureMaterial
    d = d
    Gc = 1e-3
    L = 0.01
    local_dissipation_norm = 0.5
  [../]
  [./degradation]
    type = DerivativeParsedMaterial
    f_name = 'g_d'
    args = 'd'
    constant_names = 'eta'
    constant_expressions = '1e-6'
    function = '(1-d)^2*(1-eta)+eta'
    derivative_order = 2
  [../]
  [./local]
    type = DerivativeParsedMaterial
    f_name = 'w_d'
    args = 'd'
    function = 'd^2'
    derivative_order = 2
  [../]
  [./driving_energy]
    type = DerivativeSumMaterial
    args = 'd'
    derivative_order = 2
    f_name = 'D_d'
    sum_materials = 'E_el_d w_d'
  [../]
  [./critical_fracture_energy]
    type = FractureEnergyBarrier
    initial_degradation_slope = -2
    initial_local_dissipation_slope = 0
    d = d
  [../]

  [./elasticity_tensor]
    type = ComputeElasticityTensor
    C_ijkl = '120.0 80.0'
    fill_method = symmetric_isotropic
  [../]
  [./strain]
    type = ComputeSmallStrain
    displacements = 'disp_x disp_y'
  [../]
  [./stress]
    type = SmallStrainElasticDegradedStress
    damage_fields = 'd'
  [../]
  [./lump]
    type = LumpedDegradation
    damage_fields = 'd'
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
    ksp_norm = default
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -snes_linesearch_type -sub_pc_factor_levels'
  petsc_options_value = 'asm      ilu          200         200                basic                 0'
  dt = 1e-5
  end_time = 5e-3
[]

[Outputs]
  print_linear_residuals = false
  exodus = true
  perf_graph = true
[]

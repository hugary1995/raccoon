[Mesh]
  type = FileMesh
  file = 'mesh/geo.e'
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
    f_name = D_d
    mob_name = 'mobility_d'
  [../]
  [./ACInterface]
    type = ACInterface
    variable = d
    kappa_name = 'kappa_d'
    mob_name = 'mobility_d'
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
    type = PFFracBulkMaterial
    d = d
    Gc = 1e-3
    L = 0.01
    local_dissipation_norm = 0.5
  [../]
  [./degradation]
    type = Degradation
    f_name = 'g_d'
    args = 'd'
    constant_names       = 'eta'
    constant_expressions = '0'
    function = '(1-d)^2*(1-eta)+eta'
  [../]
  [./local]
    type = LocalDissipation
    f_name = 'w_d'
    args = 'd'
    function = 'd^2'
  [../]
  [./lump]
    type = LumpedDegradation
    damage_fields = 'd'
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
    type = ComputeDegradedStress
    damage_fields = 'd'
    decomposition = strain_spectral
    history = false
  [../]
  [./driving_energy]
    type = FractureDrivingForce
    damage_fields = 'd'
    local_dissipation_names = 'w_d'
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
  petsc_options_iname = '-ksp-type -pc_type'
  petsc_options_value = 'preonly   lu'
  dt = 1e-5
  end_time = 5e-3
[]

[Outputs]
  exodus = true
  perf_graph = true
[]

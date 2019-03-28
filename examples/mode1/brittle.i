[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = FileMesh
  file = 'gold/geo.e'
[]

[Modules]
  [./PhaseFieldFracture]
    [./BrittleFracture]
      [./d]
        block = 1
        viscosity = 1e-12
        Gc = 1e-3
        L = 0.02
      [../]
    [../]
    [./ElasticCoupling]
      [./all]
        damage_fields = 'd'
        strain = FINITE
        decomposition = STRAIN_SPECTRAL
        irreversibility = HISTORY
        block = 1
      [../]
    [../]
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
  [./elasticity_tensor]
    type = ComputeElasticityTensor
    C_ijkl = '120.0 80.0'
    fill_method = symmetric_isotropic
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
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type'
  petsc_options_value = 'asm      ilu          200         200                0                     newtontr'
  dt = 1e-5
  end_time = 5e-3
[]

[Outputs]
  print_linear_residuals = false
  exodus = true
[]

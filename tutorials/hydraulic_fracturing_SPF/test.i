[Problem]
  solve = false
[]

[Mesh]
  [generated_mesh]
    type = GeneratedMeshGenerator
    dim = 2
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

[Materials]
  [crack_opening]
    type = ComputeCrackOpeningDisplacement
    phase_field = d
  []
  [strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  nl_max_its = 50
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8
  dt = 1e-1
  end_time = 1
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  automatic_scaling = true
[]

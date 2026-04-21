[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 2
    ny = 2
  []
[]

[Variables]
  [u]
  []
[]

[Kernels]
  [diff]
    type = Diffusion
    variable = u
  []
[]

[BCs]
  [fix]
    type = DirichletBC
    variable = u
    boundary = 'left right top bottom'
    value = 0
  []
[]

[AuxVariables]
  [d]
  []
[]

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = sub.i
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [from_d]
    type = MultiAppCopyTransfer
    from_multi_app = fracture
    variable = d
    source_variable = d
  []
[]

[Postprocessors]
  [d_fp_change]
    type = SolutionChangeNormFixedPoint
    variable = d
  []
[]

[Convergence]
  [fp_conv]
    type = PostprocessorConvergence
    postprocessor = d_fp_change
    tolerance = 1e-8
    max_iterations = 1
    converge_at_max_iterations = true
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  num_steps = 2
  dt = 1
  multiapp_fixed_point_convergence = fp_conv
[]

[Outputs]
  csv = true
[]

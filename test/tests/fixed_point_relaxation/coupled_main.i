# Two-way coupled fixed point, used to verify that relaxation actually damps the iteration
# rather than silently doing nothing.
#
# All-Neumann diffusion + reaction gives uniform fields satisfying the algebraic relations
#
#   main:  u = 0.5*d + 1
#   sub:   d = 0.5*u
#
# so the fixed-point map on d is d <- 0.25*d + 0.5, with rate 0.25 and fixed point d = 2/3.
# Relaxation changes the rate to 1 - alpha*0.75 but leaves the fixed point unchanged, so a
# relaxed run must reach the same d with a different iteration count.

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

[AuxVariables]
  [d]
  []
[]

[Kernels]
  [diff]
    type = Diffusion
    variable = u
  []
  [reaction]
    type = Reaction
    variable = u
  []
  [from_d]
    type = CoupledForce
    variable = u
    v = d
    coef = 0.5
  []
  [source]
    type = BodyForce
    variable = u
    value = 1
  []
[]

[MultiApps]
  [sub]
    type = TransientMultiApp
    input_files = coupled_sub.i
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [from_sub_d]
    type = MultiAppCopyTransfer
    from_multi_app = sub
    variable = d
    source_variable = d
  []
  [to_sub_u]
    type = MultiAppCopyTransfer
    to_multi_app = sub
    variable = u
    source_variable = u
  []
[]

[Postprocessors]
  # Converged fixed-point value: must be identical with and without relaxation
  [d_avg]
    type = ElementAverageValue
    variable = d
  []
  # Iteration count: must differ, proving relaxation is actually damping the update
  [n_fp_its]
    type = NumFixedPointIterations
  []
  [d_fp_change]
    type = SolutionChangeNormFixedPoint
    variable = d
    print_history = false
  []
[]

[Convergence]
  [fp_conv]
    type = PostprocessorConvergence
    postprocessor = d_fp_change
    tolerance = 1e-6
    max_iterations = 100
    converge_at_max_iterations = false
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  line_search = none
  num_steps = 1
  dt = 1
  multiapp_fixed_point_convergence = fp_conv
[]

[Outputs]
  csv = true
[]

# Verifies the effect of sub-app relaxation on the converged fixed-point solution.
#
# The sub-app drives 'd' to a uniform value of t by Dirichlet BC, independently of the main
# app, so the fixed-point map f is constant within a time step and the converged value of d
# is known analytically:
#
#   no relaxation                      -> d = t
#   relaxation, default restore        -> d = alpha*t + (1-alpha)*d_previous_step   (biased)
#   relaxation, keep_solution_during_restore -> d = t   (correct)
#
# The bias in the middle case arises because the sub-app is restored to its previous time
# step state at the start of every fixed-point iteration, so the vector that relaxation
# blends against is d from the previous time step rather than the previous iterate.

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
  # The converged fixed-point value being compared across relaxation settings
  [d_avg]
    type = ElementAverageValue
    variable = d
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
    tolerance = 1e-10
    max_iterations = 50
    converge_at_max_iterations = false
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
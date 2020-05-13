# This input file defines a coupled PDE system:
# - (g(d) * u,x),x - b1 = 0
# - d,xx - u - b2 = 0
# g(d) = (1- d) ^ 2
# b1 = - 4 * x ^ 3 + 6 * x ^ 2 - 6 * x + 2
# b2 = - x + 2
# u(0) = 0, u(1) = 1
# v(0) = 0, v(1) = 0
# u = x, d = - x ^ 2 + x
# Each step the mesh is uniformly refined once.
# The manufactured solution should have a convergence order of 2.

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 1
[]

[Adaptivity]
  marker = 'box'
  [./Markers]
    [./box]
      type = BoxMarker
      bottom_left = '0 0 0'
      top_right = '1 1 0 '
      inside = 'refine'
      outside = 'do_nothing'
    [../]
  [../]
[]

[Problem]
  type = FixedPointProblem
[]

[UserObjects]
  [./coupled_u]
    type = ADFPIMaterialPropertyUserObject
    mat_prop = 'coupled_u'
  [../]
[]

[Variables]
  [./u]
  [../]
  [./d]
  [../]
[]

[Functions]
  [./solution_u]
    type = ParsedFunction
    value = 'x'
  [../]
  [./solution_d]
    type = ParsedFunction
    value = '-x^2+x'
  [../]
[]

[Kernels]
  [./diff_u]
    type = ADCoefMatDiffusion
    variable = 'u'
    prop_names = 'g'
  [../]
  [./body_u]
    type = BodyForce
    variable = 'u'
    function = '-4*x^3+6*x^2-6*x+2'
  [../]
  [./diff_d]
    type = Diffusion
    variable = 'd'
  [../]
  [./source_d]
    type = ADMaterialPropertyUserObjectSource
    variable = 'd'
    uo_name = 'coupled_u'
    coef = -1
  [../]
  [./body_d]
    type = BodyForce
    variable = 'd'
    function = '-x+2'
  [../]
[]

[BCs]
  [./left_u]
    type = FunctionDirichletBC
    variable = 'u'
    boundary = 'left'
    function = 'solution_u'
  [../]
  [./right_u]
    type = FunctionDirichletBC
    variable = 'u'
    boundary = 'right'
    function = 'solution_u'
  [../]
  [./left_d]
    type = FunctionDirichletBC
    variable = 'd'
    boundary = 'left'
    function = 'solution_d'
  [../]
  [./right_d]
    type = FunctionDirichletBC
    variable = 'd'
    boundary = 'right'
    function = 'solution_d'
  [../]
[]

[Materials]
  [./v]
    type = QuadraticDegradation
    d = 'd'
    residual_degradation = 0
  [../]
  [./u]
    type = LinearLocalDissipation
    local_dissipation_name = 'coupled_u'
    d = 'u'
  [../]
[]

[Postprocessors]
  [./l2error]
    type = ElementL2Error
    variable = 'd'
    function = 'solution_d'
  [../]
  [./nl_its]
    type = NumNonlinearIterations
    accumulate_over_step = true
  [../]
[]

[Executioner]
  type = FixedPointTransient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  num_steps = 5

  fp_max_its = 10
  fp_tol = 1e-06

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-08
[]

[Outputs]
  exodus = true
[]

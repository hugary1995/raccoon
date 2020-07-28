# The equation we solve is
#   rho * u,tt - u,xx - f = 0
# rho = 0.5
# The unique, exact solution should be u = t^2 * x
# The velocity should be v = u,t = 2 * t * x
# The kinetic energy density is ke = 1/2 * rho * v * v = t^2 * x^2
# The kinetic energy is KE = \int_{-1}^1 ke dx = 2/3 * t^2

[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = -1
  xmax = 1
  nx = 200
  elem_type = EDGE2
[]

[Functions]
  [ic]
    type = ParsedFunction
    value = 0
  []
  [forcing_fn]
    type = ParsedFunction
    value = x
  []
  [solution_u]
    type = ParsedFunction
    value = t*t*x
  []
[]

[Variables]
  [u]
    [InitialCondition]
      type = FunctionIC
      function = ic
    []
  []
[]

[Kernels]
  [ie]
    type = InertialForce
    variable = u
  []
  [diff]
    type = Diffusion
    variable = u
  []
  [ffn]
    type = BodyForce
    variable = u
    function = forcing_fn
  []
[]

[Materials]
  [density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '0.5'
  []
[]

[BCs]
  [all]
    type = FunctionDirichletBC
    variable = u
    preset = false
    boundary = '0 1'
    function = solution_u
  []
[]

[Postprocessors]
  [l2_err]
    type = ElementL2Error
    variable = u
    function = solution_u
  []
  [KE]
    type = KineticEnergy
    density = 'density'
    displacements = 'u'
  []
[]

[Executioner]
  type = Transient

  start_time = 0.0
  num_steps = 100
  dt = 0.00005

  [TimeIntegrator]
  []
[]

[Outputs]
  [csv]
    type = CSV
  []
  [console]
    type = Console
    max_rows = 10
  []
[]

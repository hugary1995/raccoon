# The equation we solve is
#   rho * u,tt - u,xx - f = 0
# rho = 1, E = 1
# The unique, exact solution should be u = sin(x*t)
# The strain is eps = u,x = t*cos(x*t)
# The strain energy density is se = 1/2 * E * eps^2  = 0.5*t^2*cos(x*t)^2
# The strain energy is SE = \int_{0}^pi se dx =  t/8(2pi * t+ sin(2pi*t))

[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 3
  nx = 200
  elem_type = EDGE2
[]

[Functions]
  [forcing_fn]
    type = ParsedFunction
    value = '(t^2 - x^2) * sin(x*t)'
  []
  [solution_u]
    type = ParsedFunction
    value = 'sin(x*t)'
  []
[]

[Variables]
  [u]
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
    prop_values = '1'
  []
  [stress]
    type = ADComputeLinearElasticStress
  []
  [strain]
    type = ADComputeSmallStrain
    displacements = 'u'
  []
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 1
    poissons_ratio = 0
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
  [SE]
    type = ADStrainEnergy
  []
[]

[Executioner]
  type = Transient

  start_time = 0.0
  num_steps = 100
  dt = 0.000005

  nl_abs_tol = 1e-9

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

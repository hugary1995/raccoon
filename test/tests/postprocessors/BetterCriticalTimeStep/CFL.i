[Problem]
  solve = false
[]

[GlobalParams]
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = -1
  xmax = 1
  nx = 200
  elem_type = EDGE2
[]

[Materials]
  [const]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '1'
  []
  [elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = '1'
    poissons_ratio = '0'
  []

[]

[Postprocessors]
  [explicit_dt]
    type = BetterCriticalTimeStep
    execute_on = 'INITIAL TIMESTEP_BEGIN TIMESTEP_END'
  []
[]

[Executioner]
  type = Transient
  dt = 1
  num_steps = 2
[]

[Outputs]
  print_linear_residuals = false
  [CSV]
    type = CSV

  []
[]

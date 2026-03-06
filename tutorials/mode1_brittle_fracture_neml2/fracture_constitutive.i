# Free energy
[Models]
  [degradation]
    type = PowerDegradationFunction
    phase = 'forces/d'
    degradation = 'state/g'
    power = 2
    eta = 1e-6
  []
  [crack_geom]
    type = CrackGeometricFunctionAT2
    phase = 'forces/d'
    crack = 'state/alpha'
  []
  [crack_surface_energy]
    type = ScalarLinearCombination
    from_var = 'state/alpha'
    to_var = 'state/psif'
    coefficients = '1'
    coefficient_as_parameter = true
  []
  [sum]
    type = ScalarLinearCombination
    from_var = 'forces/psie_active state/psif'
    to_var = 'state/psi'
    coefficients = 'degradation ${scale}'
    coefficient_as_parameter = 'true false'
  []
  [energy]
    type = ComposedModel
    models = 'crack_geom crack_surface_energy sum'
  []
  [micro_force]
    type = Normality
    model = 'energy'
    function = 'state/psi'
    from = 'forces/d'
    to = 'state/m'
  []
  [model]
    type = ComposedModel
    models = 'energy micro_force'
  []
[]

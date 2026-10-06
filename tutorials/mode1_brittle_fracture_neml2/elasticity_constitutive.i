# Free energy
[Models]
  [degradation]
    type = PowerDegradationFunction
    phase = 'forces/d'
    degradation = 'state/g'
    power = 2
    eta = 1e-6
  []
  [elastic_energy]
    type = LinearIsotropicStrainEnergyDensity
    strain = 'forces/E'
    active_strain_energy_density = 'state/psie_active'
    inactive_strain_energy_density = 'state/psie_inactive'
    coefficient_types = 'YOUNGS_MODULUS POISSONS_RATIO'
    coefficients = '2.1e5 0.3'
    decomposition = 'NONE'
  []
  [sum]
    type = ScalarLinearCombination
    from = 'state/psie_active state/psie_inactive'
    to = 'state/psi'
    weights = 'degradation 1'
    weight_as_parameter = 'true false'
  []
  [energy]
    type = ComposedModel
    models = ' elastic_energy sum'
    additional_outputs = 'state/psie_active'
  []
  [stress]
    type = Normality
    model = 'energy'
    function = 'state/psi'
    from = 'forces/E'
    to = 'state/S'
  []
  [model]
    type = ComposedModel
    models = 'energy stress'
  []
[]
